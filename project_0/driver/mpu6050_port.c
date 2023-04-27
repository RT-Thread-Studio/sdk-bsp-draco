/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-27     Joyce       the first version
 */

#include <rtthread.h>

#ifdef PKG_USING_MPU6XXX

#include <mpu6xxx.h>
#include <sensor.h>

int rt_hw_mpu6xxx_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.dev_name = "i2c1";
    cfg.intf.user_data = (void *)RT_NULL;
    cfg.irq_pin.pin = 29;

    rt_hw_mpu6xxx_init("mpu", &cfg);
    return 0;
}
INIT_APP_EXPORT(rt_hw_mpu6xxx_port);

#endif
