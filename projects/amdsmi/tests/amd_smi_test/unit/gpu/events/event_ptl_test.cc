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

// ---------------- amdsmi_get_violation_status ----------------
TEST(GpuUnit, GetViolationStatus_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_violation_status", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_violation_status(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetViolationStatus_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_violation_status_t info;
  memset(&info, 0, sizeof(info));
  DISPLAY_AMDSMI_API("amdsmi_get_violation_status", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_violation_status(kInvalidHandle, &info);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetViolationStatus_AllGpus) {
  GTEST_SKIP() << "GetViolationStatus_AllGpus fails with error 43, AMDSMI_STATUS_UNEXPECTED_DATA";

  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_violation_status");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_violation_status_t info;
    memset(&info, 0, sizeof(info));
    DISPLAY_AMDSMI_API("amdsmi_get_violation_status", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_violation_status(dev.gpus()[i], &info);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_ptl_state ----------------
TEST(GpuUnit, GetPtlState_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ptl_state", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ptl_state(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetPtlState_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  bool enabled = false;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ptl_state", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ptl_state(kInvalidHandle, &enabled);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetPtlState_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_ptl_state");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    bool enabled = false;
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_ptl_state", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_ptl_state(dev.gpus()[i], &enabled);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_ptl_formats ----------------
TEST(GpuUnit, GetPtlFormats_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ptl_formats", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ptl_formats(dev.gpus()[0], nullptr, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetPtlFormats_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_ptl_data_format_t f1, f2;
  memset(&f1, 0, sizeof(f1));
  memset(&f2, 0, sizeof(f2));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ptl_formats", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ptl_formats(kInvalidHandle, &f1, &f2);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetPtlFormats_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_ptl_formats");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_ptl_data_format_t f1, f2;
    memset(&f1, 0, sizeof(f1));
    memset(&f2, 0, sizeof(f2));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_ptl_formats", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_ptl_formats(dev.gpus()[i], &f1, &f2);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_event_notification (no handle) ----------------
TEST(GpuUnit, GetEventNotification_NullNumElem) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_event_notification", "num_elem=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_event_notification(0, nullptr, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetEventNotification_Valid) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_evt_notification_data_t data[8];
  memset(data, 0, sizeof(data));
  uint32_t num_elem = 8;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_event_notification", "timeout=0", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_event_notification(0, &num_elem, data);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                        AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                        AMDSMI_STATUS_INIT_ERROR, AMDSMI_STATUS_INVAL);
  AMDSMI_EXPECT_STATUS(err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                       AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_INIT_ERROR,
                       AMDSMI_STATUS_INVAL);
}

// ---------------- amdsmi_set_gpu_event_notification_mask (SET) ----------------
TEST(GpuUnit, SetEventNotificationMask_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_event_notification_mask", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_event_notification_mask(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetEventNotificationMask_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_event_notification_mask");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_event_notification_mask", "gpu=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_event_notification_mask(dev.gpus()[i], 0);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NOT_INIT);
    amdsmi_col.Record(
        "gpu=" + std::to_string(i), err,
        ::amdsmi::unittest::AmdsmiStatusIsExpected(
            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
            AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_NO_PERM, AMDSMI_STATUS_NOT_INIT));
    amdsmi_stop_gpu_event_notification(dev.gpus()[i]);
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_stop_gpu_event_notification (action) ----------------
TEST(GpuUnit, StopEventNotification_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_stop_gpu_event_notification", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_stop_gpu_event_notification(kInvalidHandle);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, StopEventNotification_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_stop_gpu_event_notification");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_stop_gpu_event_notification", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_stop_gpu_event_notification(dev.gpus()[i]);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_INIT_ERROR);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_INIT_ERROR));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_set_gpu_ptl_state (SET, read/restore) ----------------
TEST(GpuUnit, SetPtlState_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_ptl_state", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_ptl_state(kInvalidHandle, false);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetPtlState_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_ptl_state");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    bool enabled = false;
    if (amdsmi_get_gpu_ptl_state(dev.gpus()[i], &enabled) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_ptl_state", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_ptl_state(dev.gpus()[i], enabled);
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

// ---------------- amdsmi_set_gpu_ptl_formats (SET, read/restore) ----------------
TEST(GpuUnit, SetPtlFormats_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_ptl_formats", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_ptl_formats(kInvalidHandle, AMDSMI_PTL_DATA_FORMAT_F32,
                                                   AMDSMI_PTL_DATA_FORMAT_F32);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetPtlFormats_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_ptl_formats");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_ptl_data_format_t f1 = AMDSMI_PTL_DATA_FORMAT_INVALID;
    amdsmi_ptl_data_format_t f2 = AMDSMI_PTL_DATA_FORMAT_INVALID;
    if (amdsmi_get_gpu_ptl_formats(dev.gpus()[i], &f1, &f2) != AMDSMI_STATUS_SUCCESS) continue;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_ptl_formats", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_ptl_formats(dev.gpus()[i], f1, f2);
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
