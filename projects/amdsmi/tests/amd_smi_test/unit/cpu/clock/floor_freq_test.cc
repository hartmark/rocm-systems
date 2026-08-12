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

#include <cstdint>
#include <cstring>
#include <string>

#include "unit/unit_test_framework.h"

using amdsmi::unittest::kInvalidHandle;
using amdsmi::unittest::kVerbose;

// CPU floor-frequency, effective-floor, MSR-floor and SDPS limit APIs. Core
// variants derive a core index from the handle (dev.cpu_cores()); socket variants
// use dev.cpus(). Write APIs guard only the handle, so they omit the null-output
// test and restore benign values in the valid path.

// ---- amdsmi_get_cpu_core_floor_freq_limit (output guarded, core handle) ----
TEST(CpuUnit, GetCoreFloorFreqLimit_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_floor_freq_limit", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_floor_freq_limit(dev.cpu_cores()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetCoreFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t floor = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_floor_freq_limit(kInvalidHandle, &floor);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetCoreFloorFreqLimit_AllCores) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_core_floor_freq_limit");
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  for (size_t i = 0; i < dev.cpu_cores().size(); ++i) {
    uint32_t floor = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_floor_freq_limit", "core=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_core_floor_freq_limit(dev.cpu_cores()[i], &floor);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("core=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_floor_freq_limit (output guarded, socket) ----
TEST(CpuUnit, GetFloorFreqLimit_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_floor_freq_limit", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_floor_freq_limit(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t floor = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_floor_freq_limit(kInvalidHandle, &floor);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetFloorFreqLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_floor_freq_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t floor = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_floor_freq_limit", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_floor_freq_limit(dev.cpus()[i], &floor);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_core_eff_floor_freq_limit (output guarded, core handle) ----
TEST(CpuUnit, GetCoreEffFloorFreqLimit_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_eff_floor_freq_limit", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_eff_floor_freq_limit(dev.cpu_cores()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetCoreEffFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t eff = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_eff_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_eff_floor_freq_limit(kInvalidHandle, &eff);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetCoreEffFloorFreqLimit_AllCores) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_core_eff_floor_freq_limit");
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  for (size_t i = 0; i < dev.cpu_cores().size(); ++i) {
    uint32_t eff = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_eff_floor_freq_limit", "core=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_core_eff_floor_freq_limit(dev.cpu_cores()[i], &eff);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("core=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_eff_floor_freq_limit (output guarded, socket) ----
TEST(CpuUnit, GetEffFloorFreqLimit_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_eff_floor_freq_limit", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_eff_floor_freq_limit(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetEffFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t eff = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_eff_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_eff_floor_freq_limit(kInvalidHandle, &eff);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetEffFloorFreqLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_eff_floor_freq_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t eff = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_eff_floor_freq_limit", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_eff_floor_freq_limit(dev.cpus()[i], &eff);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_sdps_limit (output guarded, socket) ----
TEST(CpuUnit, GetSdpsLimit_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_sdps_limit", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_sdps_limit(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetSdpsLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t sdps = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_sdps_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_sdps_limit(kInvalidHandle, &sdps);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSdpsLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_sdps_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t sdps = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_sdps_limit", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_sdps_limit(dev.cpus()[i], &sdps);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- write APIs (handle guarded only); restore benign values ----
TEST(CpuUnit, SetCoreFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_core_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_core_floor_freq_limit(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetCoreFloorFreqLimit_AllCores) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_core_floor_freq_limit");
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  for (size_t i = 0; i < dev.cpu_cores().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_core_floor_freq_limit", "core=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_core_floor_freq_limit(dev.cpu_cores()[i], 0);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NO_HSMP_MSG_SUP);
    amdsmi_col.Record("core=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM,
                          AMDSMI_STATUS_NO_HSMP_MSG_SUP));
  }
  amdsmi_col.ExpectNoFailures();
}
TEST(CpuUnit, SetFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_floor_freq_limit(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetFloorFreqLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_floor_freq_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_floor_freq_limit", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_floor_freq_limit(dev.cpus()[i], 0);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NO_HSMP_MSG_SUP);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM,
                          AMDSMI_STATUS_NO_HSMP_MSG_SUP));
  }
  amdsmi_col.ExpectNoFailures();
}
TEST(CpuUnit, SetMsrFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_msr_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_msr_floor_freq_limit(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetMsrFloorFreqLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_msr_floor_freq_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_msr_floor_freq_limit", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_msr_floor_freq_limit(dev.cpus()[i], 0);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NO_HSMP_MSG_SUP);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM,
                          AMDSMI_STATUS_NO_HSMP_MSG_SUP));
  }
  amdsmi_col.ExpectNoFailures();
}
TEST(CpuUnit, SetCoreMsrFloorFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_core_msr_floor_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_core_msr_floor_freq_limit(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetCoreMsrFloorFreqLimit_AllCores) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_core_msr_floor_freq_limit");
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  for (size_t i = 0; i < dev.cpu_cores().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_core_msr_floor_freq_limit", "core=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_core_msr_floor_freq_limit(dev.cpu_cores()[i], 0);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NO_HSMP_MSG_SUP);
    amdsmi_col.Record("core=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM,
                          AMDSMI_STATUS_NO_HSMP_MSG_SUP));
  }
  amdsmi_col.ExpectNoFailures();
}
TEST(CpuUnit, SetSdpsLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_sdps_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_sdps_limit(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetSdpsLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_sdps_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t sdps = 0;
    if (amdsmi_get_cpu_sdps_limit(dev.cpus()[i], &sdps) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_sdps_limit",
                       "cpu=" + std::to_string(i) + " sdps=" + std::to_string(sdps), kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_sdps_limit(dev.cpus()[i], sdps);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NO_HSMP_MSG_SUP);
    amdsmi_col.Record("cpu=" + std::to_string(i) + " sdps=" + std::to_string(sdps), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM,
                          AMDSMI_STATUS_NO_HSMP_MSG_SUP));
  }
  amdsmi_col.ExpectNoFailures();
}
