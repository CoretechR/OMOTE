#pragma once
#include <cstdint>
#include "lvgl.h"
class DisplayInterface 
{
    public:
        virtual void setBrightness(uint8_t brightness) = 0;
        virtual void turnOff() = 0;
    
    protected:
        virtual void pushPixel(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t* pixel_values) = 0;
        virtual void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) = 0;
};