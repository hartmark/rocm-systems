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

#include <cstring>
#include <string>

#include "unit/unit_test_framework.h"

using amdsmi::unittest::kInvalidHandle;
using amdsmi::unittest::kVerbose;

// amdsmi_get_gpu_process_isolation / amdsmi_set_gpu_process_isolation.
TEST(GpuFunctionalReadWrite, SetProcessIsolation_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_process_isolation", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_process_isolation(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(GpuFunctionalReadWrite, ProcessIsolation_SetVerifyRestore) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  amdsmi::unittest::StatusCollector col("amdsmi_set_gpu_process_isolation");
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint32_t initial = 0;
    if (amdsmi_get_gpu_process_isolation(dev.gpus()[i], &initial) != AMDSMI_STATUS_SUCCESS)
      continue;

    uint32_t target = initial ? 0u : 1u;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_process_isolation",
                       "gpu=" + std::to_string(i) + " set=" + std::to_string(target), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_process_isolation(dev.gpus()[i], target);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM);
    col.Record("gpu=" + std::to_string(i), err,
               ::amdsmi::unittest::AmdsmiStatusIsExpected(
                   err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                   AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM));

    if (err == AMDSMI_STATUS_SUCCESS) {
      uint32_t readback = 0;
      if (amdsmi_get_gpu_process_isolation(dev.gpus()[i], &readback) == AMDSMI_STATUS_SUCCESS) {
        EXPECT_EQ(readback, target) << "gpu=" << i << " set did not take effect";
      }
      amdsmi_status_t rerr = amdsmi_set_gpu_process_isolation(dev.gpus()[i], initial);
      DISPLAY_AMDSMI_API("amdsmi_set_gpu_process_isolation",
                         "gpu=" + std::to_string(i) + " restore=" + std::to_string(initial),
                         kVerbose);
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, rerr, AMDSMI_STATUS_SUCCESS);
      EXPECT_EQ(rerr, AMDSMI_STATUS_SUCCESS)
          << "gpu=" << i << " failed to restore process isolation";
      if (rerr == AMDSMI_STATUS_SUCCESS &&
          amdsmi_get_gpu_process_isolation(dev.gpus()[i], &readback) == AMDSMI_STATUS_SUCCESS) {
        EXPECT_EQ(readback, initial) << "gpu=" << i << " restore did not take effect";
      }
    }
  }
  col.ExpectNoFailures();
}
