# Handover: ROCm/rocm-systems#12431 — 100% CPU spin in `InterruptSignal::WaitRelaxed`

Session context for picking this up in a new session. Written 2026-09-29.

## TL;DR

- Issue https://github.com/ROCm/rocm-systems/issues/12431 (reporter: x90050) is a
  persistent 99.9% single-core spin in `rocr::core::InterruptSignal::WaitRelaxed`
  on ROCm 10.0 userland, gfx1201 (Radeon AI PRO R9700), vLLM 0.30.
- It cross-references hartmark's open PR **ROCm/rocm-systems#11170**
  ("perf(rocr): back off BusyWaitSignal::WaitRelaxed to a yielding poll").
  That PR only touches `BusyWaitSignal::WaitRelaxed` (`default_signal.cpp`),
  **not** `InterruptSignal`, so it does not fix this issue — it's the same pattern
  in a sibling class.
- Also referenced: PR #7898 (by seanthegeek, merged 2026-08-18) — the
  `AsyncEventsLoop` backoff that introduced `core/util/poll_backoff.h`. Reporter
  showed that fix is live and the `WaitRelaxed` spin persists.
- Nothing has been changed, committed or pushed yet. Only analysis.

## Issue facts (from the public issue page)

- Env: Ubuntu 24.04, kernel 6.11, amdgpu DKMS 7.1.3 (31.50.0), ROCm userland 10.0
  (`libhsa-runtime64.so.1.21.0`, `libamdhip64.so.7`), vLLM 0.30.0 in Docker,
  PyTorch 2.14, FP8 KV cache.
- Stack:
  ```
  Thread N (LWP <tid> "VLLM::EngineCor"):
  #0 rocr::core::InterruptSignal::WaitRelaxed(...)        libhsa-runtime64.so.1
  #1 rocr::core::InterruptSignal::WaitAcquire(...)        libhsa-runtime64.so.1
  #2 rocr::HSA::hsa_signal_wait_scacquire(...)            libhsa-runtime64.so.1
  #3-5 ?? (stripped)                                      libamdhip64.so.7
  #6 start_thread
  ```
  -> a HIP-internal thread (created by libamdhip64); name inherited from parent.
- strace: 374 syscalls in 6 s, **all `gettid` (~62/s)**; no
  `AMDKFD_IOC_WAIT_EVENTS`, no futex. ~100% userspace.
- Spins from init, idle, and under load. Present on ROCm 10.0; not on 7.14 / 7.2.3
  (those only show the AsyncEventsLoop spin).
- `GPU_MAX_HW_QUEUES`, `VLLM_SLEEP_WHEN_IDLE` no effect. Forcing
  `HSA_WAIT_STATE_BLOCKED` via an LD_PRELOAD interposer: no effect.
- Reporter's questions:
  1. Which libamdhip64 path enters `WaitRelaxed` with `ACTIVE` and never returns?
  2. Is userspace polling intended for `ACTIVE`, and why no iteration cap/backoff?
  3. Should `wait_state_hint` be honored on this path?

## Code analysis (against `develop` in hartmark/rocm-systems — NOT verified against ROCm 10.0 sources)

### `projects/rocr-runtime/runtime/hsa-runtime/core/runtime/interrupt_signal.cpp:142` — `InterruptSignal::WaitRelaxed`

Two ways the loop spins with zero syscalls:

1. **`ACTIVE` hint = unbounded `continue` loop.** If `g_use_mwaitx` is false
   (e.g. Intel host / mwaitx disabled) it's a pure hot spin; even with mwaitx it's
   a 1000-cycle mwaitx then `continue`. No cap, no backoff.
   - HIP defaults every device to active wait: `projects/clr/hipamd/src/hip_context.cpp:52`
     (`SetActiveWait(true)`); `hipSetDeviceFlags` handling in
     `projects/clr/hipamd/src/hip_device_runtime.cpp:~826`.
   - clr `WaitForSignal()` (`projects/clr/rocclr/device/rocm/rocvirtual.hpp:55`)
     then waits with `HSA_WAIT_STATE_ACTIVE` in 4 s windows.
   - Also: `if (!event_age && prior != 0) wait_hint = HSA_WAIT_STATE_ACTIVE;`
     (~line 153) forces ACTIVE when KFD lacks event_age and another waiter exists.

2. **Null KFD event → spin regardless of hint (best fit for "BLOCKED has no effect").**
   - `InterruptSignal` ctor (~line 94) may get `event_ == nullptr` when
     `EventPool::alloc()` (~line 50) fails (KFD signal-event slots exhausted →
     `allEventsAllocated = true`, returns nullptr).
   - After the 200 µs window the loop calls `hsaKmtWaitOnEvent_Ext(event_, ...)`;
     `projects/rocr-runtime/libhsakmt/src/events.c:263` returns
     `HSAKMT_STATUS_INVALID_HANDLE` immediately for a null event — **no ioctl**.
     Return value is ignored; loop continues → 100% CPU, no syscalls.

### Which HIP thread? — likely the hostcall listener

- `projects/clr/rocclr/device/devhostcall.cpp`, old impl (`!USE_NEW_HOSTCALL_IMPL`):
  `consumePackets()` waits on the doorbell with timeout ramping up to
  `kTimeoutCeil = K*K*16` ns = **16 ms → 62.5 wakes/s**, matching the ~62/s
  `gettid` rate. Each wait call spins the whole 16 ms window if case 1 or 2 applies.
- Doorbell signal wait state is `Blocked` on ROCm Linux (`initDevice`, ~line 565),
  so case 2 (null event) or the `prior`/`event_age` override is more likely than
  a plain ACTIVE hint for this thread. Unconfirmed.
- Hostcall listener starts when a kernel uses hostcall (device printf / malloc / assert).
- `USE_NEW_HOSTCALL_IMPL` is set via `projects/clr/rocclr/cmake/ROCclr.cmake:92`.
  Which impl ROCm 10.0 ships is unknown.

### Why the interposer doesn't bite

clr resolves ROCr symbols through its own table (`ROCR_DYN` / `GET_ROCR_SYMBOL`,
`projects/clr/rocclr/device/rocm/rocrctx.hpp:152`), so an LD_PRELOAD override of
the exported `hsa_signal_wait_scacquire` likely never sees clr's calls.

## Suggested next steps

1. Confirm against ROCm 10.0 sources: `interrupt_signal.cpp`, `devhostcall.cpp`
   (which hostcall impl + timeouts), `libhsakmt/src/events.c`.
2. Fix idea (follow-up to #11170, same `core/util/poll_backoff.h` helpers):
   - In `InterruptSignal::WaitRelaxed`, when `event_ == nullptr` (or
     `hsaKmtWaitOnEvent_Ext` returns non-success/non-timeout), fall back to the
     backoff nap (`os::uSleep` with `NextPollNapUs` ramp) instead of hot looping.
   - Bound the `ACTIVE` branch: hot-spin window (e.g. `kHotPollActiveUs` = 10 ms,
     as in #11170), then backoff / fall through to the event wait.
   - Add unit tests alongside `projects/rocr-runtime/rocrtst/common/utils_test/poll_backoff_test.cpp`.
3. Optionally reply on #12431 with the null-event hypothesis and ask the reporter to
   check: KFD event count / `HSAKMT` debug logs, whether `hsaKmtWaitOnEvent_Ext`
   returns `INVALID_HANDLE`, and `perf` symbols inside `WaitRelaxed`.

## Environment notes for the next session

- Working fork: `hartmark/rocm-systems`, branch `claude/github-issue-12431-h4jr42`
  (currently no changes vs develop besides this file).
- A session with `hartmark/rocm-systems` attached **cannot** also attach
  `ROCm/rocm-systems` (same dir name), so GitHub API reads on the upstream issue/PR
  fail with 403. Use a session sourced from `ROCm/rocm-systems`, or read the public
  pages via WebFetch.
- Related upstream refs: PR #11170 (hartmark, open, approved by cfreeamd, awaiting
  code owners), PR #11171 (companion `madvise(MADV_WILLNEED)` PR),
  ROCm/TheRock#7832, PR #7898.
