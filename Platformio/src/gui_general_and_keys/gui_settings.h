#ifndef __GUI_SETTINGS_H__
#define __GUI_SETTINGS_H__

#include <lvgl.h>

extern lv_obj_t* objBattSettingsVoltage;
extern lv_obj_t* objBattSettingsPercentage;
//extern lv_obj_t* objBattSettingsIscharging;

void init_gui_settings(lv_obj_t* tabview);
void init_gui_pageIndicator_settings();

#endif /*__GUI_SETTINGS_H__*/
