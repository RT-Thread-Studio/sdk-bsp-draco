#if 0
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
//#include "systick.h"
#include "sensor.h"
#include "plic.h"
#include "dvp.h"
#include "fpioa.h"
#include "sysctl.h"
#include "i2c.h"
#include "sleep.h"
#include <rtthread.h>
#include "cambus.h"

static int g_dvp_finish_flag = 0;
static int sensor_irq(void *ctx)
{
    if (g_dvp_finish_flag == 1)
    {
        dvp_config_interrupt(DVP_CFG_START_INT_ENABLE | DVP_CFG_FINISH_INT_ENABLE, 0);
    }

    // sensor_t *sensor = ctx;
    if (dvp_get_interrupt(DVP_STS_FRAME_FINISH))
    { //frame end
        dvp_clear_interrupt(DVP_STS_FRAME_START | DVP_STS_FRAME_FINISH);
        g_dvp_finish_flag = 1;
    }
    else
    {                               //frame start
        if (g_dvp_finish_flag == 0) //only we finish the convert, do transmit again
            dvp_start_convert();    //so we need deal img ontime, or skip one framebefore next
        dvp_clear_interrupt(DVP_STS_FRAME_START);
    }
    //rt_kprintf("D\n");
    return 0;
}

omvhal_cambus_t cambus;
omvhal_cambus_t *bus = &cambus;
extern int cambus_read_id(uint8_t addr, uint16_t *manuf_id, uint16_t *device_id);
extern int cambus_read16_id(uint8_t addr, uint16_t *manuf_id, uint16_t *device_id);

void get_chip_id(int slv_addr){
    int chip_id = 0;
    switch (slv_addr) {
        case GC0308_ADDR<<1: // Or OV9650.
            cambus_readb(&cambus, slv_addr, GC0308_CHIP_ID, &chip_id);
            rt_kprintf("GC0308-ID:0x%x\n", chip_id);
            break;
        case OV2640_SLV_ADDR: // Or OV9650.
            cambus_readb(&cambus, slv_addr, OV_CHIP_ID, &chip_id);
            break;
        case OV5640_SLV_ADDR:
            cambus_readb2(&cambus, slv_addr, OV5640_CHIP_ID, &chip_id);
            break;
        // case OV7725_SLV_ADDR: // Or OV7690.
        //     cambus_readb(&cambus, slv_addr, OV_CHIP_ID, &chip_id);
        //     break;
        case MT9V034_SLV_ADDR:
            cambus_readb(&cambus, slv_addr, ON_CHIP_ID, &chip_id);
            break;
        case LEPTON_SLV_ADDR:
            chip_id = LEPTON_ID;
            break;
        case HM01B0_SLV_ADDR:
            cambus_readb2(&cambus, slv_addr, HIMAX_CHIP_ID, &chip_id);
            break;
        default:
            return -3;
            break;
    }
    rt_kprintf("CHIP ID:0x%x\n", chip_id);
}

int cinit()
{
    /* Init DVP IO map and function settings */
    fpioa_set_function(BSP_CAMERA_CMOS_RST_PIN, FUNC_CMOS_RST);
    fpioa_set_function(BSP_CAMERA_CMOS_PWDN_PIN, FUNC_CMOS_PWDN);
    fpioa_set_function(BSP_CAMERA_CMOS_XCLK_PIN, FUNC_CMOS_XCLK);
    fpioa_set_function(BSP_CAMERA_CMOS_VSYNC_PIN, FUNC_CMOS_VSYNC);
    fpioa_set_function(BSP_CAMERA_CMOS_HREF_PIN, FUNC_CMOS_HREF);
    fpioa_set_function(BSP_CAMERA_CMOS_PCLK_PIN, FUNC_CMOS_PCLK);
    fpioa_set_function(BSP_CAMERA_SCCB_SCLK_PIN, FUNC_SCCB_SCLK);
    fpioa_set_function(BSP_CAMERA_SCCB_SDA_PIN, FUNC_SCCB_SDA);

    sysctl_set_spi0_dvp_data(1);

    bus->reg_len = 8;
    dvp_init(bus->reg_len);

    dvp_set_xclk_rate(40000000);
    dvp_enable_burst();
    dvp_set_output_enable(0, 1);
    dvp_set_output_enable(1, 1);

    //dvp_set_image_size(320, 240);
    plic_set_priority(IRQN_DVP_INTERRUPT, 1);
    plic_irq_register(IRQN_DVP_INTERRUPT, sensor_irq, (void *)&bus);
    //plic_irq_enable(IRQN_DVP_INTERRUPT);
    dvp_disable_auto();
    return 0;
}
MSH_CMD_EXPORT(cinit, camera sample);

int cscan()
{
    uint16_t manuf_id = 0;
    uint16_t device_id = 0;

    bus->reg_len = 8;
    for (uint8_t addr = 0x08; addr <= 0x7f; addr++)
    {
        if (cambus_read_id(addr, &manuf_id, &device_id) != 0)
            continue;
        if (device_id != 0 && device_id != 0xffff)
        {
            rt_kprintf("addr:0x%x\n", addr);
            get_chip_id(addr);
            return addr;
        }
    }

    bus->reg_len = 16;
    for (uint8_t addr = 0x08; addr <= 0x7f; addr++)
    {
        if (cambus_read16_id(addr, &manuf_id, &device_id) != 0)
            continue;
        if (device_id != 0 && device_id != 0xffff)
        {
            return addr;
        }
    }

    return 0;
}
MSH_CMD_EXPORT(cscan, camera sample);
#endif
