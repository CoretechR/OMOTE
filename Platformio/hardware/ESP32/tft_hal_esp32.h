#pragma once

#include <LovyanGFX.hpp>

extern uint8_t LCD_BL_GPIO;
extern uint8_t LCD_EN_GPIO;
extern uint8_t LCD_CS_GPIO;
extern uint8_t LCD_MOSI_GPIO;
extern uint8_t LCD_SCK_GPIO;
extern uint8_t LCD_DC_GPIO;

// used in lvgl_hal.cpp "void my_disp_flush(..."
class LGFX : public lgfx::LGFX_Device{
private:
    lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Touch_FT5x06 _touch_instance;

public:
    LGFX(void);
};
extern LGFX tft;

// only called from lvgl_hal.cpp, not from the HAL
void init_tft(void);

// called from the HAL
void update_backligthBrighness_HAL(void);
uint8_t get_backlightBrightness_HAL();
void set_backlightBrightness_HAL(uint8_t aBacklightBrightness);
