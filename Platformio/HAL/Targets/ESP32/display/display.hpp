#pragma once
#include "DisplayAbstract.h"
#include "HardwareAbstract.hpp"
#include "Notification.hpp"
#include <Adafruit_FT6206.h>
#include <memory>
#include <TFT_eSPI.h>
#include "driver/ledc.h"

/*LEDC Channel to use for the LCD backlight*/
#define LCD_BACKLIGHT_LEDC_CHANNEL LEDC_CHANNEL_5

#define LCD_BACKLIGHT_LEDC_FREQUENCY 640

#define LCD_BACKLIGHT_LEDC_BIT_RESOLUTION 8

#define DEFAULT_BACKLIGHT_BRIGHTNESS 128


class Display: public DisplayAbstract
{
    public:
        static std::shared_ptr<Display> getInstance(std::shared_ptr<HardwareAbstract> aHardware);
        
        virtual void setBrightness(uint8_t brightness) override;
        virtual void turnOff() override;
    
        void onTouch(Notification<TS_Point>::HandlerTy aTouchHandler);

    protected:
        virtual void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
        virtual void screenInput(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) override;
        
    private:
        Display(int backlight_pin, int enable_pin, std::shared_ptr<HardwareAbstract> aHardware);
        void setupTFT();
        void setupTouchScreen();
        
        int mEnablePin;
        int mBacklightPin;
        TFT_eSPI tft;

        Adafruit_FT6206 touch;
        TS_Point touchPoint;
        TS_Point oldPoint;
        std::shared_ptr<HardwareAbstract> mHardware; 
        Notification<TS_Point> mTouchEvent;
};