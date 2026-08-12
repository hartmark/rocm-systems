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

// Socket-level CPU identity, version and capability APIs. The version/identity
// getters below only guard the processor handle in the current implementation
// (the output pointer is dereferenced on success), so their negative coverage
// is the invalid-handle test; a null-output test is omitted to avoid a crash on
// CPU hardware.

// ---- amdsmi_get_cpu_hsmp_driver_version ----
TEST(CpuUnit, GetHsmpDriverVersion_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_hsmp_driver_version_t ver;
  memset(&ver, 0, sizeof(ver));
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_hsmp_driver_version", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_hsmp_driver_version(kInvalidHandle, &ver);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetHsmpDriverVersion_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_hsmp_driver_version");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    amdsmi_hsmp_driver_version_t ver;
    memset(&ver, 0, sizeof(ver));
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_hsmp_driver_version", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_hsmp_driver_version(dev.cpus()[i], &ver);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_smu_fw_version ----
TEST(CpuUnit, GetSmuFwVersion_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_smu_fw_version_t fw;
  memset(&fw, 0, sizeof(fw));
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_smu_fw_version", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_smu_fw_version(kInvalidHandle, &fw);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetSmuFwVersion_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_smu_fw_version");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    amdsmi_smu_fw_version_t fw;
    memset(&fw, 0, sizeof(fw));
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_smu_fw_version", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_smu_fw_version(dev.cpus()[i], &fw);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_hsmp_proto_ver ----
TEST(CpuUnit, GetHsmpProtoVer_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t proto = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_hsmp_proto_ver", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_hsmp_proto_ver(kInvalidHandle, &proto);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetHsmpProtoVer_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_hsmp_proto_ver");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t proto = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_hsmp_proto_ver", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_hsmp_proto_ver(dev.cpus()[i], &proto);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_model_name ----
TEST(CpuUnit, GetCpuModelName_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_cpu_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_model_name", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_model_name(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetCpuModelName_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_model_name");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    amdsmi_cpu_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_model_name", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_model_name(dev.cpus()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_prochot_status ----
TEST(CpuUnit, GetProchotStatus_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t prochot = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_prochot_status", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_prochot_status(kInvalidHandle, &prochot);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetProchotStatus_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_prochot_status");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t prochot = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_prochot_status", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_prochot_status(dev.cpus()[i], &prochot);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_first_online_core_on_cpu_socket ----
TEST(CpuUnit, FirstOnlineCore_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t core_ind = 0;
  DISPLAY_AMDSMI_API("amdsmi_first_online_core_on_cpu_socket", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_first_online_core_on_cpu_socket(kInvalidHandle, &core_ind);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, FirstOnlineCore_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_first_online_core_on_cpu_socket");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    uint32_t core_ind = 0;
    DISPLAY_AMDSMI_API("amdsmi_first_online_core_on_cpu_socket", "cpu=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_first_online_core_on_cpu_socket(dev.cpus()[i], &core_ind);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_enabled_commands (outputs guarded) ----
TEST(CpuUnit, GetEnabledCommands_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  uint32_t m0 = 0, m1 = 0, m2 = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_enabled_commands", "r_mask=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_enabled_commands(dev.cpus()[0], nullptr, &m0, &m1, &m2);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetEnabledCommands_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  bool r_mask = false;
  uint32_t m0 = 0, m1 = 0, m2 = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_enabled_commands", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_cpu_enabled_commands(kInvalidHandle, &r_mask, &m0, &m1, &m2);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetEnabledCommands_AllCpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_enabled_commands");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i) {
    bool r_mask = false;
    uint32_t m0 = 0, m1 = 0, m2 = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_cpu_enabled_commands", "cpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_cpu_enabled_commands(dev.cpus()[i], &r_mask, &m0, &m1, &m2);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("cpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---- amdsmi_get_cpu_affinity_with_scope (cpu_set guarded; loops scope) ----
static constexpr amdsmi_affinity_scope_t kAffinityScopes[] = {AMDSMI_AFFINITY_SCOPE_NODE,
                                                              AMDSMI_AFFINITY_SCOPE_SOCKET};

TEST(CpuUnit, GetAffinityWithScope_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_affinity_with_scope", "cpu_set=nullptr", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_cpu_affinity_with_scope(dev.cpus()[0], 8, nullptr, AMDSMI_AFFINITY_SCOPE_NODE);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(CpuUnit, GetAffinityWithScope_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint64_t cpu_set[8];
  memset(cpu_set, 0, sizeof(cpu_set));
  DISPLAY_AMDSMI_API("amdsmi_get_cpu_affinity_with_scope", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_cpu_affinity_with_scope(kInvalidHandle, 8, cpu_set, AMDSMI_AFFINITY_SCOPE_NODE);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(CpuUnit, GetAffinityWithScope_AllCpusAllScopes) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_cpu_affinity_with_scope");
  if (dev.cpus().empty()) GTEST_SKIP() << "No CPU processors";
  for (size_t i = 0; i < dev.cpus().size(); ++i)
    for (auto scope : kAffinityScopes) {
      uint64_t cpu_set[8];
      memset(cpu_set, 0, sizeof(cpu_set));
      DISPLAY_AMDSMI_API("amdsmi_get_cpu_affinity_with_scope",
                         "cpu=" + std::to_string(i) + " scope=" + std::to_string(scope), kVerbose);
      amdsmi_status_t err = amdsmi_get_cpu_affinity_with_scope(dev.cpus()[i], 8, cpu_set, scope);
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                            AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
      amdsmi_col.Record("cpu=" + std::to_string(i) + " scope=" + std::to_string(scope), err,
                        ::amdsmi::unittest::AmdsmiStatusIsExpected(
                            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                            AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
    }
  amdsmi_col.ExpectNoFailures();
}
