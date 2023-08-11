#pragma once
#include <cstdint>
class DisplayInterface 
{
    virtual void setup(int backlight_pin, int enable_pin) = 0;
    virtual void pushPixel(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t* pixel_values) = 0;
    virtual void turnOff() = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
};