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

// CPU frequency, active-limit, rail iso-frequency and DFC control APIs. Getters
// that only guard the handle omit the null-output test to avoid dereferencing a
// null pointer on success. The core current-frequency limit derives a core
// index from the handle and therefore iterates dev.cpu_cores().

// ---- amdsmi_get_cpu_fclk_mclk (handle guarded only) ----
TEST(CpuUnit, GetFclkMclk_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t fclk = 0, mclk = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_fclk_mclk", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_fclk_mclk(kInvalidHandle, &fclk, &mclk);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetFclkMclk_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_fclk_mclk");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t fclk = 0, mclk = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_fclk_mclk", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_fclk_mclk(dev.cpus()[i], &fclk, &mclk);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_cclk_limit (handle guarded only) ----
TEST(CpuUnit, GetCclkLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t cclk = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_cclk_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_cclk_limit(kInvalidHandle, &cclk);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetCclkLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_cclk_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t cclk = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_cclk_limit", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_cclk_limit(dev.cpus()[i], &cclk);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_socket_current_active_freq_limit (handle guarded only) ----
TEST(CpuUnit, GetSocketActiveFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint16_t freq = 0;
  char* src_type = nullptr;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_current_active_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_cpu_socket_current_active_freq_limit(kInvalidHandle, &freq, &src_type);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSocketActiveFreqLimit_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_socket_current_active_freq_limit");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint16_t freq = 0;
    char* src_type = nullptr;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_current_active_freq_limit",
                       "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err =
        amdsmi_get_cpu_socket_current_active_freq_limit(dev.cpus()[i], &freq, &src_type);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_socket_freq_range (handle guarded only) ----
TEST(CpuUnit, GetSocketFreqRange_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint16_t fmax = 0, fmin = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_freq_range", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_freq_range(kInvalidHandle, &fmax, &fmin);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSocketFreqRange_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_socket_freq_range");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint16_t fmax = 0, fmin = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_freq_range", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_socket_freq_range(dev.cpus()[i], &fmax, &fmin);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_core_current_freq_limit (handle guarded only, core handle) ----
TEST(CpuUnit, GetCoreCurrentFreqLimit_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t freq = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_current_freq_limit", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_current_freq_limit(kInvalidHandle, &freq);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetCoreCurrentFreqLimit_AllCores) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_core_current_freq_limit");
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  for (size_t i = 0; i < dev.cpu_cores().size(); ++i) {
    uint32_t freq = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_current_freq_limit", "core=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_core_current_freq_limit(dev.cpu_cores()[i], &freq);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("core=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_rail_isofreq_policy (output guarded) ----
TEST(CpuUnit, GetRailIsofreqPolicy_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_rail_isofreq_policy", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_rail_isofreq_policy(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetRailIsofreqPolicy_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint8_t policy = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_rail_isofreq_policy", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_rail_isofreq_policy(kInvalidHandle, &policy);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetRailIsofreqPolicy_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_rail_isofreq_policy");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint8_t policy = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_rail_isofreq_policy", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_rail_isofreq_policy(dev.cpus()[i], &policy);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_set_cpu_rail_isofreq_policy (write; policy pointer guarded) ----
TEST(CpuUnit, SetRailIsofreqPolicy_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_rail_isofreq_policy", "policy=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_rail_isofreq_policy(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, SetRailIsofreqPolicy_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  bool policy = false;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_rail_isofreq_policy", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_rail_isofreq_policy(kInvalidHandle, &policy);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetRailIsofreqPolicy_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_rail_isofreq_policy");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    bool policy = false;
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_rail_isofreq_policy", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_rail_isofreq_policy(dev.cpus()[i], &policy);
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

// ---- amdsmi_get_cpu_dfc_ctrl (output guarded) ----
TEST(CpuUnit, GetDfcCtrl_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_dfc_ctrl", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_dfc_ctrl(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetDfcCtrl_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint8_t dfc = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_dfc_ctrl", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_dfc_ctrl(kInvalidHandle, &dfc);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetDfcCtrl_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_dfc_ctrl");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint8_t dfc = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_dfc_ctrl", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_dfc_ctrl(dev.cpus()[i], &dfc);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_set_cpu_dfc_ctrl (write; dfc pointer guarded) ----
TEST(CpuUnit, SetDfcCtrl_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_dfc_ctrl", "dfc=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_dfc_ctrl(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, SetDfcCtrl_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint8_t dfc = 0;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_dfc_ctrl", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_dfc_ctrl(kInvalidHandle, &dfc);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetDfcCtrl_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_dfc_ctrl");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint8_t dfc = 0;
    if (amdsmi_get_cpu_dfc_ctrl(dev.cpus()[i], &dfc) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_dfc_ctrl", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_dfc_ctrl(dev.cpus()[i], &dfc);
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
