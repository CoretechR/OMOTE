#ifndef __GUI_SETTINGS_H__
#define __GUI_SETTINGS_H__

#include <lvgl.h>

extern lv_obj_t* objBattSettingsVoltage;
extern lv_obj_t* objBattSettingsPercentage;
//extern lv_obj_t* objBattSettingsIscharging;

const char * const tabName_settings = "Settings";
void register_gui_settings(void);

#endif /*__GUI_SETTINGS_H__*/
