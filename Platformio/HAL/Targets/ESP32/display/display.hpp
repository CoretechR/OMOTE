#pragma once
#include "DisplayInterface.h"
#include <TFT_eSPI.h>
#include <memory>
#include "driver/ledc.h"

/*LEDC Channel to use for the LCD backlight*/
#define LCD_BACKLIGHT_LEDC_CHANNEL LEDC_CHANNEL_5

#define LCD_BACKLIGHT_LEDC_FREQUENCY 640

#define LCD_BACKLIGHT_LEDC_BIT_RESOLUTION 8

#define DEFAULT_BACKLIGHT_BRIGHTNESS 128


class Display: public DisplayInterface
{
    public:
        static std::shared_ptr<Display> getInstance();
        
        virtual void setBrightness(uint8_t brightness) override;
        virtual void turnOff() override;
    
    protected:
        virtual void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {};
        virtual void pushPixel(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t* pixel_values) override;

    private:
        static std::shared_ptr<Display> mInstance;
        int enable_pin;
        int backlight_pin;
        Display(int backlight_pin, int enable_pin);
        TFT_eSPI tft;
};