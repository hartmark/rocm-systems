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

#define UNIT_GPU_STRUCT_GETTER(TESTBASE, APINAME, STRUCT)                                    \
  TEST(GpuUnit, TESTBASE##_NullOutput) {                                                     \
    amdsmi::unittest::UnitDevices dev;                                                       \
    if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";                             \
    DISPLAY_AMDSMI_API(#APINAME, "gpu=0 out=nullptr", kVerbose);                             \
    amdsmi_status_t err = APINAME(dev.gpus()[0], nullptr);                                   \
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL);           \
    EXPECT_EQ(err, AMDSMI_STATUS_INVAL);                                                     \
  }                                                                                          \
  TEST(GpuUnit, TESTBASE##_InvalidHandle) {                                                  \
    amdsmi::unittest::UnitDevices dev;                                                       \
    STRUCT info;                                                                             \
    memset(&info, 0, sizeof(info));                                                          \
    DISPLAY_AMDSMI_API(#APINAME, "handle=invalid", kVerbose);                                \
    amdsmi_status_t err = APINAME(kInvalidHandle, &info);                                    \
    DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_INVAL,            \
                          AMDSMI_STATUS_NOT_SUPPORTED);                                      \
    EXPECT_NE(err, AMDSMI_STATUS_SUCCESS);                                                   \
  }                                                                                          \
  TEST(GpuUnit, TESTBASE##_AllGpus) {                                                        \
    amdsmi::unittest::UnitDevices dev;                                                       \
    if (dev.gpus().empty()) GTEST_SKIP() << "No GPU processors";                             \
    for (size_t i = 0; i < dev.gpus().size(); ++i) {                                         \
      STRUCT info;                                                                           \
      memset(&info, 0, sizeof(info));                                                        \
      DISPLAY_AMDSMI_API(#APINAME, "gpu=" + std::to_string(i), kVerbose);                    \
      amdsmi_status_t err = APINAME(dev.gpus()[i], &info);                                   \
      DISPLAY_AMDSMI_STATUS(kVerbose, __FILE__, __LINE__, err, AMDSMI_STATUS_SUCCESS,        \
                            AMDSMI_STATUS_NOT_SUPPORTED, AMDSMI_STATUS_NOT_YET_IMPLEMENTED); \
    }                                                                                        \
  }

UNIT_GPU_STRUCT_GETTER(GetDriverInfo, amdsmi_get_gpu_driver_info, amdsmi_driver_info_t)
UNIT_GPU_STRUCT_GETTER(GetAsicInfo, amdsmi_get_gpu_asic_info, amdsmi_asic_info_t)
UNIT_GPU_STRUCT_GETTER(GetKfdInfo, amdsmi_get_gpu_kfd_info, amdsmi_kfd_info_t)
UNIT_GPU_STRUCT_GETTER(GetVramInfo, amdsmi_get_gpu_vram_info, amdsmi_vram_info_t)
UNIT_GPU_STRUCT_GETTER(GetBoardInfo, amdsmi_get_gpu_board_info, amdsmi_board_info_t)
UNIT_GPU_STRUCT_GETTER(GetVbiosInfo, amdsmi_get_gpu_vbios_info, amdsmi_vbios_info_t)
UNIT_GPU_STRUCT_GETTER(GetFwInfo, amdsmi_get_fw_info, amdsmi_fw_info_t)
