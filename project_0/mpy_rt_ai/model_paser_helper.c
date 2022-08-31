/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-29     liqiwen       the first version
 */

#include <rtthread.h>

#ifdef MICROPYTHON_USING_USEREXTMODS

#include <rt_ai.h>

#include "py/qstr.h"
#include "py/obj.h"
#include "py/runtime.h"

#include <backend_k210_kpu.h>

rt_ai_t backend_k210_kpu_constructor_helper(void *buffer, char *name){

    // backend_k210_kpu
    k210_kpu_t new_k210_kpu = m_new_obj(struct k210_kpu);
    new_k210_kpu->model = buffer;
    new_k210_kpu->dmac = DMAC_CHANNEL5;

    // rt_ai_t
    rt_ai_t handle = &(new_k210_kpu->parent);
    handle->info.input_n = 0;
    handle->info.output_n = 0;
    handle->info.flag = ALLOC_INPUT_BUFFER_FLAG;
    rt_strncpy(handle->parent.name, name, RT_AI_NAME_MAX);
    backend_k210_kpu((void*)new_k210_kpu);

    return handle;
}

#endif
