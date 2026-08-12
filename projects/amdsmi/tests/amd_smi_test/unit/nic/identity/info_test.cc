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
#include <vector>

#include "unit/unit_test_framework.h"

using amdsmi::unittest::kInvalidHandle;
using amdsmi::unittest::kVerbose;

// =====================================================================
// amdsmi_get_nic_processor_handles(socket, uint32_t* count, handles*)
// =====================================================================

TEST(NicUnit, GetNicProcessorHandles_NullCount) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.sockets().empty()) GTEST_SKIP() << "No sockets";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_processor_handles", "socket=0 count=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_processor_handles(dev.sockets()[0], nullptr, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicProcessorHandles_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t count = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_nic_processor_handles", "socket=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_processor_handles(nullptr, &count, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicProcessorHandles_AllSockets) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_processor_handles");
  if (dev.sockets().empty()) GTEST_SKIP() << "No sockets";
  for (size_t i = 0; i < dev.sockets().size(); ++i) {
    uint32_t count = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_nic_processor_handles",
                       "socket=" + std::to_string(i) + " count-query", kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_processor_handles(dev.sockets()[i], &count, nullptr);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("socket=" + std::to_string(i) + " count-query", err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
    if (err != AMDSMI_STATUS_SUCCESS || count == 0) continue;
    std::vector<amdsmi_processor_handle> handles(count);
    DISPLAY_AMDSMI_API("amdsmi_get_nic_processor_handles", "socket=" + std::to_string(i) + " fill",
                       kVerbose);
    err = amdsmi_get_nic_processor_handles(dev.sockets()[i], &count, handles.data());
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("socket=" + std::to_string(i) + " fill", err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_device_bdf(handle, amdsmi_bdf_t* bdf)
// =====================================================================

TEST(NicUnit, GetNicDeviceBdf_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_device_bdf", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_device_bdf(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicDeviceBdf_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_bdf_t bdf;
  memset(&bdf, 0, sizeof(bdf));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_device_bdf", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_device_bdf(kInvalidHandle, &bdf);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicDeviceBdf_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_device_bdf");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_bdf_t bdf;
    memset(&bdf, 0, sizeof(bdf));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_device_bdf", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_device_bdf(dev.nics()[i], &bdf);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_driver_info(handle, amdsmi_nic_driver_info_t* info)
// =====================================================================

TEST(NicUnit, GetNicDriverInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_driver_info", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_driver_info(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicDriverInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_nic_driver_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_driver_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_driver_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicDriverInfo_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_driver_info");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_nic_driver_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_driver_info", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_driver_info(dev.nics()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_asic_info(handle, amdsmi_nic_asic_info_t* info)
// =====================================================================

TEST(NicUnit, GetNicAsicInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_asic_info", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_asic_info(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicAsicInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_nic_asic_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_asic_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_asic_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicAsicInfo_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_asic_info");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_nic_asic_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_asic_info", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_asic_info(dev.nics()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_bus_info(handle, amdsmi_nic_bus_info_t* info)
// =====================================================================

TEST(NicUnit, GetNicBusInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_bus_info", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_bus_info(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicBusInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_nic_bus_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_bus_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_bus_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicBusInfo_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_bus_info");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_nic_bus_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_bus_info", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_bus_info(dev.nics()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_numa_info(handle, amdsmi_nic_numa_info_t* info)
// =====================================================================

TEST(NicUnit, GetNicNumaInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_numa_info", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_numa_info(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicNumaInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_nic_numa_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_numa_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_numa_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicNumaInfo_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_numa_info");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_nic_numa_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_numa_info", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_numa_info(dev.nics()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_port_info(handle, amdsmi_nic_port_info_t* info)
// =====================================================================

TEST(NicUnit, GetNicPortInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_port_info", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_port_info(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicPortInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_nic_port_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_port_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_port_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicPortInfo_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_port_info");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_nic_port_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_port_info", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_port_info(dev.nics()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_rdma_dev_info(handle, amdsmi_nic_rdma_devices_info_t* info)
// =====================================================================

TEST(NicUnit, GetNicRdmaDevInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_rdma_dev_info", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_rdma_dev_info(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicRdmaDevInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_nic_rdma_devices_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_rdma_dev_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_rdma_dev_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicRdmaDevInfo_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_rdma_dev_info");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_nic_rdma_devices_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_rdma_dev_info", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_rdma_dev_info(dev.nics()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// =====================================================================
// amdsmi_get_nic_fw_info(handle, amdsmi_nic_fw_info_t* info)
// =====================================================================

TEST(NicUnit, GetNicFwInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  DISPLAY_AMDSMI_API("amdsmi_get_nic_fw_info", "nic=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_fw_info(dev.nics()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}

TEST(NicUnit, GetNicFwInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_nic_fw_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_nic_fw_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_nic_fw_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}

TEST(NicUnit, GetNicFwInfo_AllNics) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_nic_fw_info");
  if (dev.nics().empty()) GTEST_SKIP() << "No NIC devices";
  for (size_t i = 0; i < dev.nics().size(); ++i) {
    amdsmi_nic_fw_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_nic_fw_info", "nic=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_nic_fw_info(dev.nics()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("nic=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}
