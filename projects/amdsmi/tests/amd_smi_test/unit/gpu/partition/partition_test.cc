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

// ---------------- amdsmi_get_gpu_compute_partition (char) ----------------
TEST(GpuUnit, GetComputePartition_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_compute_partition", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_compute_partition(dev.gpus()[0], nullptr, 64);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetComputePartition_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  char buf[64];
  memset(buf, 0, sizeof(buf));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_compute_partition", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_compute_partition(kInvalidHandle, buf, sizeof(buf));
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetComputePartition_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_compute_partition");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    char buf[64];
    memset(buf, 0, sizeof(buf));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_compute_partition", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_compute_partition(dev.gpus()[i], buf, sizeof(buf));
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_memory_partition (char) ----------------
TEST(GpuUnit, GetMemoryPartition_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_memory_partition", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_memory_partition(dev.gpus()[0], nullptr, 64);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetMemoryPartition_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  char buf[64];
  memset(buf, 0, sizeof(buf));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_memory_partition", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_memory_partition(kInvalidHandle, buf, sizeof(buf));
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetMemoryPartition_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_memory_partition");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    char buf[64];
    memset(buf, 0, sizeof(buf));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_memory_partition", "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_memory_partition(dev.gpus()[i], buf, sizeof(buf));
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_compute_partition_mem_alloc_mode ----------------
TEST(GpuUnit, GetComputePartitionMemAllocMode_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_compute_partition_mem_alloc_mode", "gpu=0 out=nullptr",
                     kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_compute_partition_mem_alloc_mode(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetComputePartitionMemAllocMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_compute_partition_mem_alloc_mode_t mode;
  memset(&mode, 0, sizeof(mode));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_compute_partition_mem_alloc_mode", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_compute_partition_mem_alloc_mode(kInvalidHandle, &mode);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetComputePartitionMemAllocMode_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_compute_partition_mem_alloc_mode");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_compute_partition_mem_alloc_mode_t mode;
    memset(&mode, 0, sizeof(mode));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_compute_partition_mem_alloc_mode",
                       "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_compute_partition_mem_alloc_mode(dev.gpus()[i], &mode);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_accelerator_partition_mem_alloc_mode ----------------
TEST(GpuUnit, GetAcceleratorPartitionMemAllocMode_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_mem_alloc_mode", "gpu=0 out=nullptr",
                     kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_accelerator_partition_mem_alloc_mode(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetAcceleratorPartitionMemAllocMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_accelerator_partition_mem_alloc_mode_t mode;
  memset(&mode, 0, sizeof(mode));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_mem_alloc_mode", "handle=invalid",
                     kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_accelerator_partition_mem_alloc_mode(kInvalidHandle, &mode);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetAcceleratorPartitionMemAllocMode_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col(
      "amdsmi_get_gpu_accelerator_partition_mem_alloc_mode");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_accelerator_partition_mem_alloc_mode_t mode;
    memset(&mode, 0, sizeof(mode));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_mem_alloc_mode",
                       "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_accelerator_partition_mem_alloc_mode(dev.gpus()[i], &mode);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_memory_partition_config ----------------
TEST(GpuUnit, GetMemoryPartitionConfig_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_memory_partition_config", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_memory_partition_config(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetMemoryPartitionConfig_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_memory_partition_config_t config;
  memset(&config, 0, sizeof(config));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_memory_partition_config", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_memory_partition_config(kInvalidHandle, &config);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetMemoryPartitionConfig_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_memory_partition_config");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_memory_partition_config_t config;
    memset(&config, 0, sizeof(config));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_memory_partition_config", "gpu=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_memory_partition_config(dev.gpus()[i], &config);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_accelerator_partition_profile_config ----------------
TEST(GpuUnit, GetAcceleratorPartitionProfileConfig_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_profile_config", "gpu=0 out=nullptr",
                     kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_accelerator_partition_profile_config(dev.gpus()[0], nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_ARG_PTR_NULL);
  AMDSMI_EXPECT_NULL_ARG(err);
}
TEST(GpuUnit, GetAcceleratorPartitionProfileConfig_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_accelerator_partition_profile_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_profile_config", "handle=invalid",
                     kVerbose);
  amdsmi_status_t err = amdsmi_get_gpu_accelerator_partition_profile_config(kInvalidHandle, &cfg);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetAcceleratorPartitionProfileConfig_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col(
      "amdsmi_get_gpu_accelerator_partition_profile_config");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_accelerator_partition_profile_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_profile_config",
                       "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_get_gpu_accelerator_partition_profile_config(dev.gpus()[i], &cfg);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- amdsmi_get_gpu_accelerator_partition_profile ----------------
TEST(GpuUnit, GetAcceleratorPartitionProfile_NullOutput) {
  amdsmi::unittest::UnitDevices dev;
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_profile", "gpu=0 out=nullptr", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_gpu_accelerator_partition_profile(dev.gpus()[0], nullptr, nullptr);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);
  EXPECT_EQ(err, AMDSMI_STATUS_INVAL);
}
TEST(GpuUnit, GetAcceleratorPartitionProfile_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi_accelerator_partition_profile_t profile;
  memset(&profile, 0, sizeof(profile));
  uint32_t partition_id = 0;
  DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_profile", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_get_gpu_accelerator_partition_profile(kInvalidHandle, &profile, &partition_id);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, GetAcceleratorPartitionProfile_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_get_gpu_accelerator_partition_profile");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_accelerator_partition_profile_t profile;
    memset(&profile, 0, sizeof(profile));
    uint32_t partition_id = 0;
    DISPLAY_AMDSMI_API("amdsmi_get_gpu_accelerator_partition_profile", "gpu=" + std::to_string(i),
                       kVerbose);
    amdsmi_status_t err =
        amdsmi_get_gpu_accelerator_partition_profile(dev.gpus()[i], &profile, &partition_id);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED);
    amdsmi_col.Record("gpu=" + std::to_string(i), err,
                      ::amdsmi::unittest::AmdsmiStatusIsExpected(
                          err, AMDSMI_STATUS_SUCCESS, AMDSMI_STATUS_NOT_SUPPORTED,
                          AMDSMI_STATUS_NOT_YET_IMPLEMENTED));
  }
  amdsmi_col.ExpectNoFailures();
}

// ---------------- SET: mem_alloc_mode setters (read current, restore) ----------------
TEST(GpuUnit, SetComputePartitionMemAllocMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_compute_partition_mem_alloc_mode", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_compute_partition_mem_alloc_mode(
      kInvalidHandle, AMDSMI_COMPUTE_PARTITION_MEM_ALLOC_CAPPING);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetComputePartitionMemAllocMode_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col("amdsmi_set_gpu_compute_partition_mem_alloc_mode");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_compute_partition_mem_alloc_mode_t mode = AMDSMI_COMPUTE_PARTITION_MEM_ALLOC_INVALID;
    if (amdsmi_get_gpu_compute_partition_mem_alloc_mode(dev.gpus()[i], &mode) !=
        AMDSMI_STATUS_SUCCESS)
      continue;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_compute_partition_mem_alloc_mode",
                       "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_compute_partition_mem_alloc_mode(dev.gpus()[i], mode);
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
TEST(GpuUnit, SetAcceleratorPartitionMemAllocMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_accelerator_partition_mem_alloc_mode", "handle=invalid",
                     kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_accelerator_partition_mem_alloc_mode(
      kInvalidHandle, AMDSMI_ACCELERATOR_PARTITION_MEM_ALLOC_CAPPING);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetAcceleratorPartitionMemAllocMode_AllGpus) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  amdsmi::unittest::StatusCollector amdsmi_col(
      "amdsmi_set_gpu_accelerator_partition_mem_alloc_mode");
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    amdsmi_accelerator_partition_mem_alloc_mode_t mode =
        AMDSMI_ACCELERATOR_PARTITION_MEM_ALLOC_INVALID;
    if (amdsmi_get_gpu_accelerator_partition_mem_alloc_mode(dev.gpus()[i], &mode) !=
        AMDSMI_STATUS_SUCCESS)
      continue;
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_accelerator_partition_mem_alloc_mode",
                       "gpu=" + std::to_string(i), kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_accelerator_partition_mem_alloc_mode(dev.gpus()[i], mode);
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

// ---------------- SET: repartition ops exercised with non-destructive sentinel inputs
// ---------------- A successful repartition would reconfigure a live device shared with other
// processes, so the valid-handle path is intentionally driven with a sentinel/
// invalid selector that the driver rejects; only the call contract is validated.
TEST(GpuUnit, SetComputePartition_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_compute_partition", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_set_gpu_compute_partition(kInvalidHandle, AMDSMI_COMPUTE_PARTITION_SPX);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetComputePartition_AllGpusSentinel) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_compute_partition", "gpu=" + std::to_string(i) + " sentinel",
                       kVerbose);
    amdsmi_status_t err =
        amdsmi_set_gpu_compute_partition(dev.gpus()[i], AMDSMI_COMPUTE_PARTITION_INVALID);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM);
  }
}
TEST(GpuUnit, SetMemoryPartition_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_memory_partition", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_set_gpu_memory_partition(kInvalidHandle, AMDSMI_MEMORY_PARTITION_NPS1);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetMemoryPartition_AllGpusSentinel) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_memory_partition", "gpu=" + std::to_string(i) + " sentinel",
                       kVerbose);
    amdsmi_status_t err =
        amdsmi_set_gpu_memory_partition(dev.gpus()[i], AMDSMI_MEMORY_PARTITION_UNKNOWN);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM);
  }
}
TEST(GpuUnit, SetMemoryPartitionMode_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_memory_partition_mode", "handle=invalid", kVerbose);
  amdsmi_status_t err =
      amdsmi_set_gpu_memory_partition_mode(kInvalidHandle, AMDSMI_MEMORY_PARTITION_NPS1);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetMemoryPartitionMode_AllGpusSentinel) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_memory_partition_mode",
                       "gpu=" + std::to_string(i) + " sentinel", kVerbose);
    amdsmi_status_t err =
        amdsmi_set_gpu_memory_partition_mode(dev.gpus()[i], AMDSMI_MEMORY_PARTITION_UNKNOWN);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM);
  }
}
TEST(GpuUnit, SetAcceleratorPartitionProfile_InvalidHandle) {
  amdsmi::unittest::UnitDevices dev;
  DISPLAY_AMDSMI_API("amdsmi_set_gpu_accelerator_partition_profile", "handle=invalid", kVerbose);
  amdsmi_status_t err = amdsmi_set_gpu_accelerator_partition_profile(kInvalidHandle, 0);
  DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                        AMDSMI_STATUS_NOT_SUPPORTED);
  EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);
}
TEST(GpuUnit, SetAcceleratorPartitionProfile_AllGpusSentinel) {
  amdsmi::unittest::UnitDevices dev;
  AMDSMI_SKIP_IF_MUTATION_DISABLED();
  if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";
  for (size_t i = 0; i < dev.gpus().size(); ++i) {
    DISPLAY_AMDSMI_API("amdsmi_set_gpu_accelerator_partition_profile",
                       "gpu=" + std::to_string(i) + " sentinel", kVerbose);
    amdsmi_status_t err = amdsmi_set_gpu_accelerator_partition_profile(dev.gpus()[i], 0xFFFFFFFF);
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,
                          AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED,
                          AMDSMI_STATUS_NO_PERM);
  }
}
