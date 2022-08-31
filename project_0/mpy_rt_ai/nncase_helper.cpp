/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-30     liqiwen       the first version
 */
#if 1
#include "model_paser_helper.h"
#include "nncase.h"

size_t inputs_size(rt_ai_t handle){
    kpu_model_context_t _ctx = ((k210_kpu_t)handle)->task;
    kpu_model_context_t *ctx = &_ctx;
    return km_inputs_size(ctx);
}

size_t outputs_size(rt_ai_t handle){
    kpu_model_context_t _ctx = ((k210_kpu_t)handle)->task;
    kpu_model_context_t *ctx = &_ctx;
    return km_outputs_size(ctx);
}

size_t inputs_n_bytes(rt_ai_t handle, size_t index){
    kpu_model_context_t _ctx = ((k210_kpu_t)handle)->task;
    kpu_model_context_t *ctx = &_ctx;
    return km_inputs_n_bytes(ctx, index);
}

size_t outputs_n_bytes(rt_ai_t handle, size_t index){
    kpu_model_context_t _ctx = ((k210_kpu_t)handle)->task;
    kpu_model_context_t *ctx = &_ctx;
    return km_outputs_n_bytes(ctx, index);
}
#endif
