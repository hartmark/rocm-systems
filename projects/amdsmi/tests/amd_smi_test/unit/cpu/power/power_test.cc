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

// CPU power, power-cap, power-efficiency and energy APIs. Socket-scoped calls
// use dev.cpus(); the core-scoped power/energy calls derive a core index from the
// handle and therefore iterate dev.cpu_cores(). Getters that only guard the handle
// dereference the output on success, so those omit the null-output test.

// ---- amdsmi_get_cpu_socket_power (output guarded) ----
TEST(CpuUnit, GetSocketPower_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_power(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetSocketPower_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t power = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_power(kInvalidHandle, &power);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSocketPower_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_socket_power");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t power = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_socket_power(dev.cpus()[i], &power);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_socket_power_cap (output guarded) ----
TEST(CpuUnit, GetSocketPowerCap_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power_cap", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_power_cap(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetSocketPowerCap_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t cap = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power_cap", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_power_cap(kInvalidHandle, &cap);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSocketPowerCap_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_socket_power_cap");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t cap = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power_cap", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_socket_power_cap(dev.cpus()[i], &cap);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_socket_power_cap_max (output guarded) ----
TEST(CpuUnit, GetSocketPowerCapMax_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power_cap_max", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_power_cap_max(dev.cpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetSocketPowerCapMax_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t cap_max = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power_cap_max", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_power_cap_max(kInvalidHandle, &cap_max);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSocketPowerCapMax_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_socket_power_cap_max");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t cap_max = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_power_cap_max", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_socket_power_cap_max(dev.cpus()[i], &cap_max);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_pwr_svi_telemetry_all_rails (handle guarded only) ----
TEST(CpuUnit, GetPwrSviTelemetryAllRails_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t power = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_pwr_svi_telemetry_all_rails", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_pwr_svi_telemetry_all_rails(kInvalidHandle, &power);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetPwrSviTelemetryAllRails_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_pwr_svi_telemetry_all_rails");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t power = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_pwr_svi_telemetry_all_rails", "cpu=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_pwr_svi_telemetry_all_rails(dev.cpus()[i], &power);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_pwr_efficiency_mode (handle guarded only) ----
TEST(CpuUnit, GetPwrEfficiencyMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t mode = 0, util = 0, ppt = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_pwr_efficiency_mode", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_pwr_efficiency_mode(kInvalidHandle, &mode, &util, &ppt);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetPwrEfficiencyMode_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_pwr_efficiency_mode");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t mode = 0, util = 0, ppt = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_pwr_efficiency_mode", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_pwr_efficiency_mode(dev.cpus()[i], &mode, &util, &ppt);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_socket_energy (handle guarded only) ----
TEST(CpuUnit, GetSocketEnergy_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint64_t energy = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_energy", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_energy(kInvalidHandle, &energy);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSocketEnergy_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_socket_energy");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint64_t energy = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_energy", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_socket_energy(dev.cpus()[i], &energy);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_socket_c0_residency (handle guarded only) ----
TEST(CpuUnit, GetSocketC0Residency_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t residency = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_c0_residency", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_socket_c0_residency(kInvalidHandle, &residency);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSocketC0Residency_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_socket_c0_residency");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t residency = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_socket_c0_residency", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_socket_c0_residency(dev.cpus()[i], &residency);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_core_ccd_power (output guarded, core handle) ----
TEST(CpuUnit, GetCoreCcdPower_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_ccd_power", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_ccd_power(dev.cpu_cores()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetCoreCcdPower_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t power = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_ccd_power", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_ccd_power(kInvalidHandle, &power);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetCoreCcdPower_AllCores) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_core_ccd_power");
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  for (size_t i = 0; i < dev.cpu_cores().size(); ++i) {
    uint32_t power = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_ccd_power", "core=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_core_ccd_power(dev.cpu_cores()[i], &power);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("core=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_core_energy (handle guarded only, core handle) ----
TEST(CpuUnit, GetCoreEnergy_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint64_t energy = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_energy", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_core_energy(kInvalidHandle, &energy);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetCoreEnergy_AllCores) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_core_energy");
  if (dev.cpu_cores().empty()) GTEST_SKIP() << "No CPU cores";
  for (size_t i = 0; i < dev.cpu_cores().size(); ++i) {
    uint64_t energy = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_core_energy", "core=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_core_energy(dev.cpu_cores()[i], &energy);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("core=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_set_cpu_socket_power_cap (write; restores current value) ----
TEST(CpuUnit, SetSocketPowerCap_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_socket_power_cap", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_socket_power_cap(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetSocketPowerCap_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_socket_power_cap");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t cap = 0;
    if (amdsmi_get_cpu_socket_power_cap(dev.cpus()[i], &cap) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_socket_power_cap",
                       "cpu=" + std::to_string(i) + " cap=" + std::to_string(cap), kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_socket_power_cap(dev.cpus()[i], cap);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NO_HSMP_MSG_SUP);
    amdsmi_col.Record("cpu=" + std::to_string(i) + " cap=" + std::to_string(cap), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM,
                          AMDSMI_STATUS_NO_HSMP_MSG_SUP));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_set_cpu_pwr_efficiency_mode (write; util/ppt outputs guarded) ----
TEST(CpuUnit, SetPwrEfficiencyMode_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  uint32_t ppt = 0;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_pwr_efficiency_mode", "utilization=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_pwr_efficiency_mode(dev.cpus()[0], 0, nullptr, &ppt);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, SetPwrEfficiencyMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t util = 0, ppt = 0;
  DISPLAY_AMDSMI_API("amdsmi_set_cpu_pwr_efficiency_mode", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_cpu_pwr_efficiency_mode(kInvalidHandle, 0, &util, &ppt);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, SetPwrEfficiencyMode_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_cpu_pwr_efficiency_mode");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t util = 0, ppt = 0;
    DISPLAY_AMDSMI_API("amdsmi_set_cpu_pwr_efficiency_mode", "cpu=" + std::to_string(i) + " mode=0",
                       kVerbose);
    amdsmi_status_t err = amdsmi_set_cpu_pwr_efficiency_mode(dev.cpus()[i], 0, &util, &ppt);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NO_HSMP_MSG_SUP);
    amdsmi_col.Record("cpu=" + std::to_string(i) + " mode=0", err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM,
                          AMDSMI_STATUS_NO_HSMP_MSG_SUP));
  }
  amdsmi_col.ExpectNoFailures();
}
