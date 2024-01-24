#ifndef __GUI_H__
#define __GUI_H__

#include <lvgl.h>
#include "tft.h"

// LVGL declarations
LV_IMG_DECLARE(high_brightness);
LV_IMG_DECLARE(low_brightness);
extern lv_obj_t* objBattPercentage;
extern lv_obj_t* objBattIcon;
extern lv_obj_t* panel;
extern lv_color_t color_primary;

extern lv_obj_t* WifiLabel;
extern lv_obj_t* BluetoothLabel;

extern byte currentScreen; // Current screen that is shown

void init_gui(void);
void gui_loop(void);

#endif /*__GUI_H__*/