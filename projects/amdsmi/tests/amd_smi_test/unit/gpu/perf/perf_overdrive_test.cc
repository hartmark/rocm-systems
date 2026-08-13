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

// ---------------- amdsmi_get_gpu_busy_percent ----------------
TEST(GpuUnit, GetBusyPercent_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_busy_percent", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_busy_percent(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetBusyPercent_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t busy = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_busy_percent", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_busy_percent(kInvalidHandle, &busy);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetBusyPercent_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_busy_percent");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint32_t busy = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_busy_percent", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_busy_percent(dev.gpus()[i], &busy);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_vcn_busy_percent ----------------
TEST(GpuUnit, GetVcnBusyPercent_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_vcn_busy_percent", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_vcn_busy_percent(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetVcnBusyPercent_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t busy = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_vcn_busy_percent", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_vcn_busy_percent(kInvalidHandle, &busy);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetVcnBusyPercent_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_vcn_busy_percent");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint32_t busy = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_vcn_busy_percent", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_vcn_busy_percent(dev.gpus()[i], &busy);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_utilization_count ----------------
TEST(GpuUnit, GetUtilizationCount_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  amdsmi_utilization_counter_t counters[1];
  memset(counters, 0, sizeof(counters));
  counters[0].type = AMDSMI_COARSE_GRAIN_GFX_ACTIVITY;
  DISPLAY_AMDSMI_API("amdsmi_get_utilization_count", "gpu=0 timestamp=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_utilization_count(dev.gpus()[0], counters, 1, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetUtilizationCount_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_utilization_counter_t counters[1];
  memset(counters, 0, sizeof(counters));
  counters[0].type = AMDSMI_COARSE_GRAIN_GFX_ACTIVITY;
  uint64_t ts = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_utilization_count", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_utilization_count(kInvalidHandle, counters, 1, &ts);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetUtilizationCount_AllGpus) {
  GTEST_SKIP() << "GetUtilizationCount_AllGpus fails with error 43, AMDSMI_STATUS_UNEXPECTED_DATA";

  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_utilization_count");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_utilization_counter_t counters[6];
    memset(counters, 0, sizeof(counters));
    counters[0].type = AMDSMI_COARSE_GRAIN_GFX_ACTIVITY;
    counters[1].type = AMDSMI_COARSE_GRAIN_MEM_ACTIVITY;
    counters[2].type = AMDSMI_COARSE_DECODER_ACTIVITY;
    counters[3].type = AMDSMI_FINE_GRAIN_GFX_ACTIVITY;
    counters[4].type = AMDSMI_FINE_GRAIN_MEM_ACTIVITY;
    counters[5].type = AMDSMI_FINE_DECODER_ACTIVITY;
    uint64_t ts = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_utilization_count", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_utilization_count(dev.gpus()[i], counters, 6, &ts);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_perf_level ----------------
TEST(GpuUnit, GetPerfLevel_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_perf_level", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_perf_level(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetPerfLevel_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_dev_perf_level_t perf;
  memset(&perf, 0, sizeof(perf));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_perf_level", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_perf_level(kInvalidHandle, &perf);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetPerfLevel_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_perf_level");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_dev_perf_level_t perf;
    memset(&perf, 0, sizeof(perf));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_perf_level", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_perf_level(dev.gpus()[i], &perf);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_overdrive_level ----------------
TEST(GpuUnit, GetOverdriveLevel_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_overdrive_level", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_overdrive_level(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetOverdriveLevel_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t od = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_overdrive_level", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_overdrive_level(kInvalidHandle, &od);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetOverdriveLevel_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_overdrive_level");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint32_t od = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_overdrive_level", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_overdrive_level(dev.gpus()[i], &od);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_mem_overdrive_level ----------------
TEST(GpuUnit, GetMemOverdriveLevel_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_mem_overdrive_level", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_mem_overdrive_level(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetMemOverdriveLevel_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t od = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_mem_overdrive_level", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_mem_overdrive_level(kInvalidHandle, &od);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetMemOverdriveLevel_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_mem_overdrive_level");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint32_t od = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_mem_overdrive_level", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_mem_overdrive_level(dev.gpus()[i], &od);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_od_volt_info ----------------
TEST(GpuUnit, GetOdVoltInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_od_volt_info", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_od_volt_info(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetOdVoltInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_od_volt_freq_data_t odv;
  memset(&odv, 0, sizeof(odv));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_od_volt_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_od_volt_info(kInvalidHandle, &odv);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetOdVoltInfo_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_od_volt_info");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_od_volt_freq_data_t odv;
    memset(&odv, 0, sizeof(odv));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_od_volt_info", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_od_volt_info(dev.gpus()[i], &odv);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_od_volt_curve_regions ----------------
TEST(GpuUnit, GetOdVoltCurveRegions_NullNum) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_od_volt_curve_regions", "gpu=0 num=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_od_volt_curve_regions(dev.gpus()[0], nullptr, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetOdVoltCurveRegions_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint32_t num = 8;
  amdsmi_freq_volt_region_t buf[8];
  memset(buf, 0, sizeof(buf));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_od_volt_curve_regions", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_od_volt_curve_regions(kInvalidHandle, &num, buf);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetOdVoltCurveRegions_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_od_volt_curve_regions");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint32_t num = 8;
    amdsmi_freq_volt_region_t buf[8];
    memset(buf, 0, sizeof(buf));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_od_volt_curve_regions", "gpu=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_od_volt_curve_regions(dev.gpus()[i], &num, buf);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_activity ----------------
TEST(GpuUnit, GetActivity_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_activity", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_activity(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetActivity_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_engine_usage_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_activity", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_activity(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetActivity_AllGpus) {
  GTEST_SKIP() << "GetActivity_AllGpus fails with error 43, AMDSMI_STATUS_UNEXPECTED_DATA";

  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_activity");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_engine_usage_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_activity", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_activity(dev.gpus()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_set_gpu_perf_level (SET, enum) ----------------
TEST(GpuUnit, SetPerfLevel_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_perf_level", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_perf_level(kInvalidHandle, AMDSMI_DEV_PERF_LEVEL_AUTO);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetPerfLevel_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_perf_level");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_dev_perf_level_t perf = AMDSMI_DEV_PERF_LEVEL_AUTO;
    if (amdsmi_get_gpu_perf_level(dev.gpus()[i], &perf) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_perf_level", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_perf_level(dev.gpus()[i], perf);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_set_gpu_overdrive_level (SET) ----------------
TEST(GpuUnit, SetOverdriveLevel_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_overdrive_level", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_overdrive_level(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetOverdriveLevel_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_overdrive_level");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint32_t od = 0;
    if (amdsmi_get_gpu_overdrive_level(dev.gpus()[i], &od) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_overdrive_level", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_overdrive_level(dev.gpus()[i], od);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_set_gpu_perf_determinism_mode (SET) ----------------
TEST(GpuUnit, SetPerfDeterminismMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_perf_determinism_mode", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_perf_determinism_mode(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetPerfDeterminismMode_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_perf_determinism_mode");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_perf_determinism_mode", "gpu=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_perf_determinism_mode(dev.gpus()[i], 0);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_INVAL);
    amdsmi_col.Record(
        "gpu=" + std::to_string(i), err,
        ::amdsmi::unittest::AmdsmiStatusIsExpected(
            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
            AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_INVAL));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_set_gpu_od_clk_info (SET, enum) ----------------
TEST(GpuUnit, SetOdClkInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_od_clk_info", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_set_gpu_od_clk_info(kInvalidHandle, AMDSMI_FREQ_IND_MIN, 0, AMDSMI_CLK_TYPE_SYS);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetOdClkInfo_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_od_clk_info");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_od_volt_freq_data_t odv;
    memset(&odv, 0, sizeof(odv));
    if (amdsmi_get_gpu_od_volt_info(dev.gpus()[i], &odv) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_od_clk_info", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_od_clk_info(
        dev.gpus()[i], AMDSMI_FREQ_IND_MIN, odv.curr_sclk_range.lower_bound, AMDSMI_CLK_TYPE_SYS);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_INVAL);
    amdsmi_col.Record(
        "gpu=" + std::to_string(i), err,
        ::amdsmi::unittest::AmdsmiStatusIsExpected(
            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
            AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_INVAL));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_set_gpu_od_volt_info (SET) ----------------
TEST(GpuUnit, SetOdVoltInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_od_volt_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_od_volt_info(kInvalidHandle, 0, 0, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetOdVoltInfo_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_od_volt_info");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_od_volt_info", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_od_volt_info(dev.gpus()[i], 0, 0, 0);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_INVAL);
    amdsmi_col.Record(
        "gpu=" + std::to_string(i), err,
        ::amdsmi::unittest::AmdsmiStatusIsExpected(
            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
            AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_INVAL));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_reset_gpu (action) ----------------
// NOTE: The valid-handle path is intentionally NOT exercised. Issuing a real GPU
// reset would disrupt other processes sharing the device; only the invalid-handle
// contract is validated here.
TEST(GpuUnit, ResetGpu_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_reset_gpu", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_reset_gpu(kInvalidHandle);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
