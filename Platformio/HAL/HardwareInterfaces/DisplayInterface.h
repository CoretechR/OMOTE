#pragma once
#include <memory> 
#include "lvgl.h"
class DisplayAbstract 
{
    public:
        DisplayAbstract();
        virtual void setBrightness(uint8_t brightness) = 0;
        virtual void turnOff() = 0;
    
    protected:
        // Set this with a getInstance method in the Child Class
        static std::shared_ptr<DisplayAbstract> mInstance; 
        
        virtual void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) = 0;
        virtual void screenInput(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) = 0;
    private:

        // Used to satisfy LVGL APIs  
        static void flushDisplayImpl(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
        static void screenInputImpl(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

        // LVGL Screen Buffers
        lv_disp_draw_buf_t mdraw_buf;
        lv_color_t mbufA[SCREEN_WIDTH * SCREEN_HEIGHT / 10];
        lv_color_t mbufB[SCREEN_WIDTH * SCREEN_HEIGHT / 10];
};