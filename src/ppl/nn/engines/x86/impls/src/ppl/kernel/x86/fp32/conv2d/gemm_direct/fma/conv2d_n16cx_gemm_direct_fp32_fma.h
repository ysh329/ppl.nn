// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef __ST_PPL_KERNEL_X86_FP32_CONV2D_GEMM_DIRECT_FMA_CONV2D_N16CX_GEMM_DIRECT_FP32_FMA_H_
#define __ST_PPL_KERNEL_X86_FP32_CONV2D_GEMM_DIRECT_FMA_CONV2D_N16CX_GEMM_DIRECT_FP32_FMA_H_

#include "ppl/kernel/x86/fp32/conv2d.h"
#include "ppl/kernel/x86/common/internal_include.h"
#include "ppl/kernel/x86/common/timer.h"

namespace ppl { namespace kernel { namespace x86 {

// forward declare;
class conv2d_n16cx_gemm_direct_fp32_fma_manager;

class conv2d_n16cx_gemm_direct_fp32_fma_executor final : public conv2d_fp32_executor {
public:
    conv2d_n16cx_gemm_direct_fp32_fma_executor() {}
    conv2d_n16cx_gemm_direct_fp32_fma_executor(const conv2d_fp32_param *conv_param, const float *cvt_filter, const float *bias)
        : conv2d_fp32_executor(conv_param, cvt_filter, bias) {}
    uint64_t cal_temp_buffer_size() override;
    ppl::common::RetCode prepare() override;
    ppl::common::RetCode execute() override;

private:
    struct kernel_schedule_param {
        // Preprocessed param
        int32_t ic_per_gp;
        int32_t oc_per_gp;
        int32_t padded_ic;
        int32_t padded_oc;

        // Kernel tunning
        int32_t hw_kr_blk;
        int32_t hw_l3_blk;
        int32_t hw_l3_buf;
        int32_t ic_l2_blk;
        int32_t ic_l2_cnt;
        int32_t ic_l3_blk;
        int32_t oc_kr_blk;
        int32_t oc_l2_blk;
        int32_t mb_l3_blk;
        int32_t gp_l3_blk;
        int32_t use_nt_store;

        // Kernel threading
        int32_t hw_l2_blk;
        int32_t cur_batch;
        int32_t cur_group;
    } schedule_param_;

    void init_preproc_param();
    void cal_kernel_tunning_param();
    void cal_kernel_threading_param(const int32_t batch, const int32_t group);

    static int32_t cal_ic_l2_blk(const conv2d_fp32_param &param);

    friend conv2d_n16cx_gemm_direct_fp32_fma_manager;
};

class conv2d_n16cx_gemm_direct_fp32_fma_manager final : public conv2d_fp32_manager {
public:
    conv2d_n16cx_gemm_direct_fp32_fma_manager() {}
    conv2d_n16cx_gemm_direct_fp32_fma_manager(const conv2d_fp32_param &param, ppl::common::Allocator *allocator)
        : conv2d_fp32_manager(param, allocator) {}
    bool is_supported() override;
    ppl::common::RetCode gen_cvt_weights(const float *filter, const float *bias) override;
    conv2d_fp32_executor *gen_executor() override;
};

}}}; // namespace ppl::kernel::x86

#endif
