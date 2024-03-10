#pragma once

#include <TFT_eSPI.h>
#include <Adafruit_FT6206.h>

extern uint8_t LCD_BL_GPIO;
extern uint8_t LCD_EN_GPIO;

// used in lvgl_hal.cpp "void my_disp_flush(..."
extern TFT_eSPI tft;

// only called from lvgl_hal.cpp, not from the HAL
void init_tft(void);
void get_touchpoint(int16_t *touchX, int16_t *touchY);

// called from the HAL
void update_backligthBrighness_HAL(void);
uint8_t get_backlightBrightness_HAL();
void set_backlightBrightness_HAL(uint8_t aBacklightBrightness);
