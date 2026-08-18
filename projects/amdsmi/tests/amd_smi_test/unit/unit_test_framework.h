/*
 * Copyright (c) Advanced Micro Devices, Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TESTS_AMD_SMI_TEST_UNIT_UNIT_TEST_FRAMEWORK_H_
#define TESTS_AMD_SMI_TEST_UNIT_UNIT_TEST_FRAMEWORK_H_

#include <gtest/gtest.h>

#include <cstdlib>
#include <string>
#include <vector>

#include "amd_smi/amdsmi.h"
#include "test_common.h"

namespace amdsmi {
namespace unittest {

// Tests always emit the DISPLAY_AMDSMI_API/STATUS trace.
inline constexpr bool kVerbose = true;

// Sentinel used for invalid-handle negative tests.
inline constexpr amdsmi_processor_handle kInvalidHandle = nullptr;

// Per-test RAII harness wrapping TestBase. Destructor shuts AMD SMI down so each
// test leaves the refcount at zero (required by the init/shutdown refcount tests).
class UnitDevices : public TestBase {
 public:
  UnitDevices() {
    const ::testing::TestInfo* info = ::testing::UnitTest::GetInstance()->current_test_info();
    const std::string full_name =
        std::string(info->test_suite_name()) + "." + std::string(info->name());
    set_title(full_name);
    set_description("Unit-level verification of the AMD SMI C API exercised by " + full_name + ".");

    uint32_t level = GetTestVerbosity();
    if (level < TestBase::VERBOSE_STANDARD) level = TestBase::VERBOSE_STANDARD;
    set_verbosity(level);

    DisplayTestInfo();                            // #### + TEST NAME + TEST DESCRIPTION
    TestBase::SetUp(AMDSMI_INIT_ALL_PROCESSORS);  // TEST SETUP + init + enumerate
    Classify();
    TestBase::Run();  // TEST EXECUTION
  }

  ~UnitDevices() override {
    DisplayResults();   // TEST RESULTS
    TestBase::Close();  // TEST CLEAN UP + amdsmi_shut_down()
  }

  UnitDevices(const UnitDevices&) = delete;
  UnitDevices& operator=(const UnitDevices&) = delete;

  bool initialized() const { return !setup_failed_; }
  const std::vector<amdsmi_socket_handle>& sockets() const { return sockets_; }
  const std::vector<amdsmi_processor_handle>& gpus() const { return gpus_; }
  const std::vector<amdsmi_processor_handle>& cpus() const { return cpus_; }
  const std::vector<amdsmi_processor_handle>& cpu_cores() const { return cpu_cores_; }
  const std::vector<amdsmi_processor_handle>& nics() const { return nics_; }

 private:
  void Classify() {
    for (uint32_t i = 0; i < num_monitor_devs_; ++i) {
      amdsmi_processor_type_t type = AMDSMI_PROCESSOR_TYPE_UNKNOWN;
      if (amdsmi_get_processor_type(processor_handles_[i], &type) != AMDSMI_STATUS_SUCCESS)
        continue;
      switch (type) {
        case AMDSMI_PROCESSOR_TYPE_AMD_GPU:
          gpus_.push_back(processor_handles_[i]);
          break;
        case AMDSMI_PROCESSOR_TYPE_AMD_CPU:
          cpus_.push_back(processor_handles_[i]);
          break;
        case AMDSMI_PROCESSOR_TYPE_AMD_CPU_CORE:
          cpu_cores_.push_back(processor_handles_[i]);
          break;
        case AMDSMI_PROCESSOR_TYPE_AMD_NIC:
        case AMDSMI_PROCESSOR_TYPE_BRCM_NIC:
          nics_.push_back(processor_handles_[i]);
          break;
        default:
          break;
      }
    }

    // Fallbacks through the dedicated enumeration APIs when the socket walk did
    // not surface CPU/NIC processors on this platform.
    if (cpus_.empty()) {
      uint32_t cpu_count = 0;
      if (amdsmi_get_cpu_handles(&cpu_count, nullptr) == AMDSMI_STATUS_SUCCESS && cpu_count > 0) {
        cpus_.resize(cpu_count);
        if (amdsmi_get_cpu_handles(&cpu_count, cpus_.data()) != AMDSMI_STATUS_SUCCESS)
          cpus_.clear();
      }
    }
    if (cpu_cores_.empty()) {
      uint32_t core_count = 0;
      if (amdsmi_get_cpucore_handles(&core_count, nullptr) == AMDSMI_STATUS_SUCCESS &&
          core_count > 0) {
        cpu_cores_.resize(core_count);
        if (amdsmi_get_cpucore_handles(&core_count, cpu_cores_.data()) != AMDSMI_STATUS_SUCCESS)
          cpu_cores_.clear();
      }
    }
    // NIC handles are socket-scoped; amdsmi_get_nic_processor_handles is NIC-specific
    // unlike the CPU/core APIs which are socketless.
    if (nics_.empty()) {
      for (auto socket : sockets_) {
        uint32_t nic_count = 0;
        if (amdsmi_get_nic_processor_handles(socket, &nic_count, nullptr) !=
                AMDSMI_STATUS_SUCCESS ||
            nic_count == 0)
          continue;
        std::vector<amdsmi_processor_handle> nic_procs(nic_count);
        if (amdsmi_get_nic_processor_handles(socket, &nic_count, nic_procs.data()) ==
            AMDSMI_STATUS_SUCCESS)
          nics_.insert(nics_.end(), nic_procs.begin(), nic_procs.end());
      }
    }
  }

  std::vector<amdsmi_processor_handle> gpus_;
  std::vector<amdsmi_processor_handle> cpus_;
  std::vector<amdsmi_processor_handle> cpu_cores_;
  std::vector<amdsmi_processor_handle> nics_;
};

// Statuses that mean a negative/invalid-input case could not be exercised on
// this platform (feature absent or insufficient privilege) rather than a real
// contract violation. Negative tests tolerate these in addition to the proper
// status the API is documented to return.
inline bool IsUntestableHere(amdsmi_status_t status) {
  return status == AMDSMI_STATUS_NOT_SUPPORTED || status == AMDSMI_STATUS_NOT_YET_IMPLEMENTED ||
         status == AMDSMI_STATUS_NO_PERM ||
         // Required kernel driver / interface not present on this system.
         status == AMDSMI_STATUS_NO_HSMP_MSG_SUP || status == AMDSMI_STATUS_NO_HSMP_SUP ||
         status == AMDSMI_STATUS_NO_HSMP_DRV || status == AMDSMI_STATUS_NO_ENERGY_DRV ||
         status == AMDSMI_STATUS_NO_MSR_DRV || status == AMDSMI_STATUS_NO_DRV ||
         status == AMDSMI_STATUS_DRIVER_NOT_LOADED || status == AMDSMI_STATUS_NON_AMD_CPU;
}

// True when `err` matches one of the caller-listed acceptable codes. Callers
// pass the same codes the test documents in DISPLAY_AMDSMI_STATUS, i.e. the
// statuses the API *should* return for this input on some supported platform.
template <typename... Args>
inline bool AmdsmiStatusIsExpected(amdsmi_status_t err, Args... expected) {
  const amdsmi_status_t codes[] = {expected...};
  for (amdsmi_status_t c : codes) {
    if (err == c) return true;
  }
  // A platform that cannot exercise the call (feature absent / no permission)
  // is tolerated in addition to the documented proper codes.
  return IsUntestableHere(err);
}

// Human-readable "N (NAME)" for a status code.
inline std::string AmdsmiStatusLabel(amdsmi_status_t err) {
  const char* name = nullptr;
  amdsmi_status_code_to_string(err, &name);
  std::string out = std::to_string(static_cast<int>(err));
  if (name != nullptr) out += std::string(" (") + name + ")";
  return out;
}

// Accumulates the inputs whose call returned an unexpected status across a
// multi-input (id / enum) loop, then fails the test once, listing every failed
// input, instead of letting a bad code pass silently.
class StatusCollector {
 public:
  explicit StatusCollector(std::string api) : api_(std::move(api)) {}

  ~StatusCollector() {
    if (total_ > 0 && !reported_)
      ADD_FAILURE() << api_ << ": StatusCollector destroyed without calling ExpectNoFailures()";
  }

  void Record(const std::string& input, amdsmi_status_t err, bool expected) {
    ++total_;
    if (!expected) {
      failures_.push_back(input + " -> returned " + AmdsmiStatusLabel(err));
    }
  }

  void ExpectNoFailures() {
    reported_ = true;
    if (failures_.empty()) return;
    std::string msg = api_ + ": " + std::to_string(failures_.size()) + " of " +
                      std::to_string(total_) + " input(s) returned an unexpected status:";
    for (const auto& f : failures_) msg += "\n    " + f;
    ADD_FAILURE() << msg;
  }

 private:
  std::string api_;
  std::size_t total_ = 0;
  bool reported_ = false;
  std::vector<std::string> failures_;
};

}  // namespace unittest
}  // namespace amdsmi

// Assert an API returned the status it SHOULD for a null pointer argument:
// AMDSMI_STATUS_INVAL (or the more specific AMDSMI_STATUS_ARG_PTR_NULL). Only a
// genuine platform limitation (unsupported / no permission) is tolerated.
#define AMDSMI_EXPECT_NULL_ARG(actual)                                                     \
  EXPECT_TRUE((actual) == AMDSMI_STATUS_INVAL || (actual) == AMDSMI_STATUS_ARG_PTR_NULL || \
              ::amdsmi::unittest::IsUntestableHere(actual))                                \
      << "returned " << static_cast<int>(actual) << ", expected AMDSMI_STATUS_INVAL"

// Non-fatal check for a single valid call: fails the test if `err` is not one
// of the listed acceptable codes (the same set passed to DISPLAY_AMDSMI_STATUS).
#define AMDSMI_EXPECT_STATUS(err, ...)                                        \
  EXPECT_TRUE(::amdsmi::unittest::AmdsmiStatusIsExpected((err), __VA_ARGS__)) \
      << "returned " << ::amdsmi::unittest::AmdsmiStatusLabel(err)            \
      << ", which is not an expected status"

// Destructive hardware writes run by default. Set AMDSMI_TEST_DISALLOW_MUTATION
// (to any value) to skip them -- e.g. when sharing hardware where mutating the
// GPU/CPU state could disturb other work or pollute read tests in the same run.
#define AMDSMI_SKIP_IF_MUTATION_DISABLED()                                       \
  do {                                                                           \
    if (std::getenv("AMDSMI_TEST_DISALLOW_MUTATION") != nullptr)                 \
      GTEST_SKIP() << "destructive write skipped; AMDSMI_TEST_DISALLOW_MUTATION" \
                      " is set (unset it to run mutating tests)";                \
  } while (0)

#endif  // TESTS_AMD_SMI_TEST_UNIT_UNIT_TEST_FRAMEWORK_H_
