#pragma once
#include "HardwareAbstractionInterface.h"
#include "lvgl.h"

#include <functional>

#define screenWidth 240
#define screenHeight 320

class HardwareRevX : public HardwareAbstractionInterface
{
public:
    HardwareRevX() : HardwareAbstractionInterface(){};

    virtual void debugPrint(std::string aDebugMessage) override
    {
    }

    virtual void sendIR() override
    {
    }

    virtual void MQTTPublish(const char *topic, const char *payload) override
    {
    }

    virtual void initLVGL(display_flush_cb aDisplayFlushCb,
                          touch_pad_read aTouchPadReadCb)
    {
        lv_init();

        lv_disp_draw_buf_init(&mdraw_buf, mbufA, mbufB, screenWidth * screenHeight / 10);

        // Initialize the display driver
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.hor_res = screenWidth;
        disp_drv.ver_res = screenHeight;
        disp_drv.flush_cb = aDisplayFlushCb;
        disp_drv.draw_buf = &mdraw_buf;
        lv_disp_drv_register(&disp_drv);

        // Initialize the touchscreen driver
        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = aTouchPadReadCb;
        lv_indev_drv_register(&indev_drv);
    }

    virtual lv_coord_t getScreenWidth() { return screenWidth; }
    virtual lv_coord_t getScreenHeight() { return screenHeight; }

    lv_disp_draw_buf_t mdraw_buf;
    lv_color_t mbufA[screenWidth * screenHeight / 10];
    lv_color_t mbufB[screenWidth * screenHeight / 10];
};