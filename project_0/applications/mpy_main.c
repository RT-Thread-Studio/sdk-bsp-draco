/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-02     liqiwen       the first version
 */

#include <rtthread.h>
#include <stdio.h>
/* Micopython */
#if defined(PKG_USING_MICROPYTHON)
extern void mpy_main(const char *filename);
volatile int msh_run = 0;
int32_t enter_python(uint32_t delay_s)
{
    int i;
    rt_kprintf("type msh to stay msh\n");
    for(i=delay_s; i>0; i--) {
        rt_thread_delay(RT_TICK_PER_SECOND);
        if(msh_run == 1) break;
    }
    while(i==0) {
        mpy_main(NULL);
    }
    return 0;
}

static int msh(int argc, char **argv)
{
    msh_run = 1;
    return 1;
}
MSH_CMD_EXPORT(msh, enter msh skip micropython);

#ifdef RT_USING_WIFI
#include <wlan_mgnt.h>
#include <wlan_prot.h>
#include <wlan_cfg.h>

static struct rt_semaphore scan_done;
struct rt_wlan_scan_result *scan_result_cache = RT_NULL;

static void wlan_scan_report_hander(int event,struct rt_wlan_buff *buff,void *parameter)
{
    struct rt_wlan_info *info = RT_NULL;
    int index = 0;
    int ret = RT_EOK;
    RT_ASSERT(event == RT_WLAN_EVT_SCAN_REPORT);
    RT_ASSERT(buff != RT_NULL);
    RT_ASSERT(parameter != RT_NULL);

    info = (struct rt_wlan_info *)buff->data;
    index = *((int *)(parameter));
    if (scan_result_cache == RT_NULL)
    {
        RT_ASSERT(index == 0);
        scan_result_cache = rt_malloc(sizeof(struct rt_wlan_scan_result) + (rt_ubase_t)sizeof(struct rt_wlan_info));
        scan_result_cache->num = 0;
        scan_result_cache->info = (struct rt_wlan_info *)(rt_ubase_t)((rt_ubase_t)&scan_result_cache->info + sizeof(struct rt_wlan_info *));
    }
    else
    {
        scan_result_cache = rt_realloc(scan_result_cache, sizeof(struct rt_wlan_scan_result) +
                              (uint32_t)sizeof(struct rt_wlan_info) * (index + 1));
        scan_result_cache->info = (struct rt_wlan_info *)(rt_ubase_t)((rt_ubase_t)&scan_result_cache->info + sizeof(struct rt_wlan_info *));
    }
    if (scan_result_cache == RT_NULL)
    {
        LOG_E("malloc failed!");
    }
    rt_memcpy(&(scan_result_cache->info)[index], info, sizeof(struct rt_wlan_info));
    scan_result_cache->num = index + 1;
    ++ *((int *)(parameter));
}

static void wlan_scan_done_hander(int event,struct rt_wlan_buff *buff,void *parameter)
{
    RT_ASSERT(event == RT_WLAN_EVT_SCAN_DONE);
    rt_sem_release(&scan_done);
}

struct rt_wlan_scan_result *rt_wlan_scan_sync(void)
{
    static int _init = 0;
    static int i = 0;

    LOG_D("start to scan ap ...");
    if (!_init)
    {
        _init = 1;
        rt_sem_init(&scan_done, "scan_done", 0 , RT_IPC_FLAG_FIFO);
    }
    rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_REPORT, wlan_scan_report_hander,&i);
    rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_DONE, wlan_scan_done_hander,RT_NULL);
    if(rt_wlan_scan() == RT_EOK)
    {
        LOG_D("the scan is started... ");
    }
    else
    {
        LOG_E("scan failed");
    }
    rt_sem_take(&scan_done, RT_WAITING_FOREVER);
    i = 0;
    return scan_result_cache;
}

void rt_wlan_scan_result_clean(void)
{
    if(scan_result_cache)
    {
        rt_free(scan_result_cache);
        scan_result_cache = 0;
    }
}
#endif

#else
int32_t enter_python(uint32_t delay_s){
    return 0;
}
#endif



