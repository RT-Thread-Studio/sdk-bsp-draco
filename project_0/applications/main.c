/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018/09/30     Bernard      The first version
 */

#include <rtthread.h>
#include <stdio.h>

int main(void)
{
    rt_kprintf("Hello, world\n");

    return 0;
}

#include <lcd.h>

/*Camera采集完成回调函数*/
volatile uint32_t g_dvp_finish_flag;
rt_err_t camera_cb(rt_device_t dev, size_t size){
    g_dvp_finish_flag = 1;
    return RT_EOK;
}
int camera_sample(void)
{

#if 1
   rt_kprintf("Hello, world\n");
   /* read一帧图像,显示与AI图像会按顺序连续排放在buffer中 */
   uint8_t* display_image = rt_malloc((240 * 320 * 2) + (240 * 320 * 3));
    /* LCD init */
   rt_kprintf("LCD init\n");
   lcd_init();
   //清屏
   lcd_clear(BLUE);
   lcd_draw_rectangle(10, 10, 100, 100,1,RED);
   /* DVP init & 摄像头初始化*/
   rt_kprintf("DVP init\n");
   rt_device_t camera_dev = rt_device_find("gc0308"); //查找摄像头设备"gc0308"
   if(!camera_dev) {
       rt_kprintf("find camera err!\n");
       return -1;
   };
   rt_device_init(camera_dev); //初始化摄像头
   rt_device_open(camera_dev,RT_DEVICE_OFLAG_RDWR); //打开摄像头,读写模式
   rt_device_set_rx_indicate(camera_dev,camera_cb); //设置read回调函数

   /* enable global interrupt ,使能全局中断*/
   sysctl_enable_irq();
   while (1){
           g_dvp_finish_flag = 0;
           /* 采集图像显示&AI由display_image地址开始连续存放 */
           rt_device_read(camera_dev,0,display_image,0);
           while (g_dvp_finish_flag == 0) {};  //等待
           rt_kprintf("dvp cap\n");
//           __pixel_reversal(display_image, 320, 240);//lcd像素交错
           //LCD显示
           lcd_draw_picture(0, 0, 320, 240, display_image);
           lcd_draw_rectangle(10, 10, 100, 100,1,RED);
   }
#endif
    return 0;
}
MSH_CMD_EXPORT(camera_sample, camera sample);
