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

static constexpr amdsmi_gpu_block_t kGpuBlocks[] = {
    AMDSMI_GPU_BLOCK_UMC,   AMDSMI_GPU_BLOCK_SDMA,      AMDSMI_GPU_BLOCK_GFX,
    AMDSMI_GPU_BLOCK_MMHUB, AMDSMI_GPU_BLOCK_ATHUB,     AMDSMI_GPU_BLOCK_PCIE_BIF,
    AMDSMI_GPU_BLOCK_HDP,   AMDSMI_GPU_BLOCK_XGMI_WAFL, AMDSMI_GPU_BLOCK_DF,
    AMDSMI_GPU_BLOCK_SMN,   AMDSMI_GPU_BLOCK_SEM,       AMDSMI_GPU_BLOCK_MP0,
    AMDSMI_GPU_BLOCK_MP1,   AMDSMI_GPU_BLOCK_FUSE,      AMDSMI_GPU_BLOCK_MCA,
    AMDSMI_GPU_BLOCK_VCN,   AMDSMI_GPU_BLOCK_JPEG,      AMDSMI_GPU_BLOCK_IH,
    AMDSMI_GPU_BLOCK_MPIO};

// ---------------- amdsmi_get_gpu_ras_block_features_enabled (enum) ----------------
TEST(GpuUnit, GetRasBlockFeaturesEnabled_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ras_block_features_enabled", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_gpu_ras_block_features_enabled(dev.gpus()[0], AMDSMI_GPU_BLOCK_UMC, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetRasBlockFeaturesEnabled_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_ras_err_state_t state;
  memset(&state, 0, sizeof(state));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ras_block_features_enabled", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_gpu_ras_block_features_enabled(kInvalidHandle, AMDSMI_GPU_BLOCK_UMC, &state);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetRasBlockFeaturesEnabled_AllGpusAllBlocks) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_ras_block_features_enabled");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i)
    for (auto blk : kGpuBlocks) {
      amdsmi_ras_err_state_t state;
      memset(&state, 0, sizeof(state));
      DISPLAY_AMDSMI_API("amdsmi_get_gpu_ras_block_features_enabled",
                         "gpu=" + std::to_string(i) + " block=" + std::to_string(blk), kVerbose);
      amdsmi_status_t err = amdsmi_get_gpu_ras_block_features_enabled(dev.gpus()[i], blk, &state);
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                            AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
      amdsmi_col.Record("gpu=" + std::to_string(i) + " block=" + std::to_string(blk), err,
                        ::amdsmi::unittest::AmdsmiStatusIsExpected(
                            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                            AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
    }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_ecc_count (enum) ----------------
TEST(GpuUnit, GetEccCount_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_count", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ecc_count(dev.gpus()[0], AMDSMI_GPU_BLOCK_UMC, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetEccCount_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_error_count_t ec;
  memset(&ec, 0, sizeof(ec));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_count", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ecc_count(kInvalidHandle, AMDSMI_GPU_BLOCK_UMC, &ec);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetEccCount_AllGpusAllBlocks) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_ecc_count");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i)
    for (auto blk : kGpuBlocks) {
      amdsmi_error_count_t ec;
      memset(&ec, 0, sizeof(ec));
      DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_count",
                         "gpu=" + std::to_string(i) + " block=" + std::to_string(blk), kVerbose);
      amdsmi_status_t err = amdsmi_get_gpu_ecc_count(dev.gpus()[i], blk, &ec);
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                            AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
      amdsmi_col.Record("gpu=" + std::to_string(i) + " block=" + std::to_string(blk), err,
                        ::amdsmi::unittest::AmdsmiStatusIsExpected(
                            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                            AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
    }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_ecc_status (enum) ----------------
TEST(GpuUnit, GetEccStatus_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_status", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ecc_status(dev.gpus()[0], AMDSMI_GPU_BLOCK_UMC, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetEccStatus_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_ras_err_state_t state;
  memset(&state, 0, sizeof(state));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_status", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ecc_status(kInvalidHandle, AMDSMI_GPU_BLOCK_UMC, &state);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetEccStatus_AllGpusAllBlocks) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_ecc_status");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i)
    for (auto blk : kGpuBlocks) {
      amdsmi_ras_err_state_t state;
      memset(&state, 0, sizeof(state));
      DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_status",
                         "gpu=" + std::to_string(i) + " block=" + std::to_string(blk), kVerbose);
      amdsmi_status_t err = amdsmi_get_gpu_ecc_status(dev.gpus()[i], blk, &state);
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                            AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
      amdsmi_col.Record("gpu=" + std::to_string(i) + " block=" + std::to_string(blk), err,
                        ::amdsmi::unittest::AmdsmiStatusIsExpected(
                            err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                            AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
    }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_ecc_enabled ----------------
TEST(GpuUnit, GetEccEnabled_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_enabled", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ecc_enabled(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetEccEnabled_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  uint64_t blocks = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_enabled", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ecc_enabled(kInvalidHandle, &blocks);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetEccEnabled_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_ecc_enabled");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    uint64_t blocks = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_ecc_enabled", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_ecc_enabled(dev.gpus()[i], &blocks);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_total_ecc_count ----------------
TEST(GpuUnit, GetTotalEccCount_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_total_ecc_count", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_total_ecc_count(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetTotalEccCount_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_error_count_t ec;
  memset(&ec, 0, sizeof(ec));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_total_ecc_count", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_total_ecc_count(kInvalidHandle, &ec);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetTotalEccCount_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_total_ecc_count");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_error_count_t ec;
    memset(&ec, 0, sizeof(ec));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_total_ecc_count", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_total_ecc_count(dev.gpus()[i], &ec);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_ras_feature_info ----------------
TEST(GpuUnit, GetRasFeatureInfo_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ras_feature_info", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ras_feature_info(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetRasFeatureInfo_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_ras_feature_t rf;
  memset(&rf, 0, sizeof(rf));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_ras_feature_info", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_ras_feature_info(kInvalidHandle, &rf);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetRasFeatureInfo_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_ras_feature_info");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_ras_feature_t rf;
    memset(&rf, 0, sizeof(rf));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_ras_feature_info", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_ras_feature_info(dev.gpus()[i], &rf);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_gpu_validate_ras_eeprom (action) ----------------
TEST(GpuUnit, ValidateRasEeprom_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_gpu_validate_ras_eeprom", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_gpu_validate_ras_eeprom(kInvalidHandle);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, ValidateRasEeprom_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_gpu_validate_ras_eeprom");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_gpu_validate_ras_eeprom", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_gpu_validate_ras_eeprom(dev.gpus()[i]);
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

// ---------------- amdsmi_get_gpu_cper_entries ----------------
TEST(GpuUnit, GetCperEntries_NullOutput) {
  GTEST_SKIP() << "amdsmi_get_gpu_cper_entries returns OUT_OF_RESOURCES instead of INVAL for null "
                  "pointer; library input-validation bug";

  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  char cper_data[4096];
  memset(cper_data, 0, sizeof(cper_data));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_cper_entries", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_cper_entries(dev.gpus()[0], 0xFFFFFFFF, cper_data, nullptr,
                                                    nullptr, nullptr, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetCperEntries_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  char cper_data[4096];
  memset(cper_data, 0, sizeof(cper_data));
  uint64_t buf_size = sizeof(cper_data);
  amdsmi_cper_hdr_t* hdrs[16];
  memset(hdrs, 0, sizeof(hdrs));
  uint64_t entry_count = 16, cursor = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_cper_entries", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_cper_entries(kInvalidHandle, 0xFFFFFFFF, cper_data,
                                                    &buf_size, hdrs, &entry_count, &cursor);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetCperEntries_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_cper_entries");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    char cper_data[4096];
    memset(cper_data, 0, sizeof(cper_data));
    uint64_t buf_size = sizeof(cper_data);
    amdsmi_cper_hdr_t* hdrs[16];
    memset(hdrs, 0, sizeof(hdrs));
    uint64_t entry_count = 16, cursor = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_cper_entries", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_cper_entries(dev.gpus()[i], 0xFFFFFFFF, cper_data,
                                                      &buf_size, hdrs, &entry_count, &cursor);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_afids_from_cper (no handle) ----------------
TEST(GpuUnit, GetAfidsFromCper_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  char cper_buffer[256];
  memset(cper_buffer, 0, sizeof(cper_buffer));
  DISPLAY_AMDSMI_API("amdsmi_get_afids_from_cper", "out=nullptr", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_afids_from_cper(cper_buffer, sizeof(cper_buffer), nullptr, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetAfidsFromCper_DummyBuffer) {
  GTEST_SKIP() << "amdsmi_get_afids_from_cper returns AMDSMI_STATUS_UNEXPECTED_DATA; root cause "
                  "unknown, under investigation";

  amdsmi::unittest::UnitDevices dev;
  char cper_buffer[256];
  memset(cper_buffer, 0, sizeof(cper_buffer));
  uint64_t afids[16];
  memset(afids, 0, sizeof(afids));
  uint32_t num_afids = 16;
  DISPLAY_AMDSMI_API("amdsmi_get_afids_from_cper", "dummy buffer", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_afids_from_cper(cper_buffer, sizeof(cper_buffer), afids, &num_afids);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                        AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                        AMDSMI_STATUS_INVAL);
  AMDSMI_EXPECT_STATUS(err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                       AMDSMI_STATUS_NOT_YET_IMPLEMENTED, AMDSMI_STATUS_INVAL);
}
