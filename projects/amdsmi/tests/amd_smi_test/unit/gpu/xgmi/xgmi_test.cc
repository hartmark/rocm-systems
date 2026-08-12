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

// ---------------- amdsmi_gpu_xgmi_error_status ----------------
TEST(GpuUnit, XgmiErrorStatus_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_gpu_xgmi_error_status", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_gpu_xgmi_error_status(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, XgmiErrorStatus_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_xgmi_status_t status;
  memset(&status, 0, sizeof(status));
  DISPLAY_AMDSMI_API("amdsmi_gpu_xgmi_error_status", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_gpu_xgmi_error_status(kInvalidHandle, &status);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, XgmiErrorStatus_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_gpu_xgmi_error_status");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_xgmi_status_t status;
    memset(&status, 0, sizeof(status));
    DISPLAY_AMDSMI_API("amdsmi_gpu_xgmi_error_status", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_gpu_xgmi_error_status(dev.gpus()[i], &status);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_xgmi_info ----------------
TEST(GpuUnit, GetXgmiInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_xgmi_info", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_xgmi_info(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetXgmiInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_xgmi_info_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_xgmi_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_xgmi_info(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetXgmiInfo_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_xgmi_info");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_xgmi_info_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_xgmi_info", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_xgmi_info(dev.gpus()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_minmax_bandwidth_between_processors ----------------
TEST(GpuUnit, MinMaxBandwidth_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  uint64_t min_bw = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_minmax_bandwidth_between_processors", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_minmax_bandwidth_between_processors(dev.gpus()[0], dev.gpus()[0],
                                                                       &min_bw, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, MinMaxBandwidth_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  uint64_t min_bw = 0, max_bw = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_minmax_bandwidth_between_processors", "src=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_minmax_bandwidth_between_processors(
      kInvalidHandle, dev.gpus()[0], &min_bw, &max_bw);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, MinMaxBandwidth_AllPairs) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_minmax_bandwidth_between_processors");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i)
    for (size_t j = 0; j < dev.gpus().size(); ++j) {
      uint64_t min_bw = 0, max_bw = 0;
      DISPLAY_AMDSMI_API("amdsmi_get_minmax_bandwidth_between_processors",
                         "src=" + std::to_string(i) + " dst=" + std::to_string(j), kVerbose);
      amdsmi_status_t err = amdsmi_get_minmax_bandwidth_between_processors(
          dev.gpus()[i], dev.gpus()[j], &min_bw, &max_bw);
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                            AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                            AMDSMI_STATUS_INVAL);
      amdsmi_col.Record("src=" + std::to_string(i) + " dst=" + std::to_string(j), err,
                        ::amdsmi::unittest::AmdsmiStatusIsExpected(
                            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                            AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_INVAL));
    }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_is_P2P_accessible ----------------
TEST(GpuUnit, IsP2PAccessible_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_is_P2P_accessible", "out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_is_P2P_accessible(dev.gpus()[0], dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, IsP2PAccessible_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  bool accessible = false;
  DISPLAY_AMDSMI_API("amdsmi_is_P2P_accessible", "src=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_is_P2P_accessible(kInvalidHandle, dev.gpus()[0], &accessible);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, IsP2PAccessible_AllPairs) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_is_P2P_accessible");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i)
    for (size_t j = 0; j < dev.gpus().size(); ++j) {
      bool accessible = false;
      DISPLAY_AMDSMI_API("amdsmi_is_P2P_accessible",
                         "src=" + std::to_string(i) + " dst=" + std::to_string(j), kVerbose);
      amdsmi_status_t err = amdsmi_is_P2P_accessible(dev.gpus()[i], dev.gpus()[j], &accessible);
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                            AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                            AMDSMI_STATUS_INVAL);
      amdsmi_col.Record("src=" + std::to_string(i) + " dst=" + std::to_string(j), err,
                        ::amdsmi::unittest::AmdsmiStatusIsExpected(
                            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                            AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_INVAL));
    }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_reset_gpu_xgmi_error (action) ----------------
TEST(GpuUnit, ResetXgmiError_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_reset_gpu_xgmi_error", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_reset_gpu_xgmi_error(kInvalidHandle);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, ResetXgmiError_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_reset_gpu_xgmi_error");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_reset_gpu_xgmi_error", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_reset_gpu_xgmi_error(dev.gpus()[i]);
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
