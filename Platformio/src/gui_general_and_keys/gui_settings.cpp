#include <lvgl.h>
#include "preferencesStorage.h"
#include "hardware/tft.h"
#include "hardware/sleep.h"
#include "hardware/memoryUsage.h"
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/guiRegistry.h"
#include "gui_general_and_keys/gui_settings.h"

lv_obj_t* objBattSettingsVoltage;
lv_obj_t* objBattSettingsPercentage;
//lv_obj_t* objBattSettingsIscharging;

// Slider Event handler
static void bl_slider_event_cb(lv_event_t* e){
  lv_obj_t * slider = lv_event_get_target(e);
  backlight_brightness = constrain(lv_slider_get_value(slider), 60, 255);
}

// Wakeup by IMU Switch Event handler
static void WakeEnableSetting_event_cb(lv_event_t* e){
  wakeupByIMUEnabled = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
}

// timout event handler
static void timout_event_cb(lv_event_t* e){
  lv_obj_t * drop = lv_event_get_target(e);
  uint16_t selected = lv_dropdown_get_selected(drop);
  switch (selected) {
    case 0: {actualSleepTimeout =   10000; break;}
    case 1: {actualSleepTimeout =   20000; break;}
    case 2: {actualSleepTimeout =   40000; break;}
    case 3: {actualSleepTimeout =   60000; break;}
    case 4: {actualSleepTimeout =  180000; break;}
    case 5: {actualSleepTimeout =  600000; break;}
    case 6: {actualSleepTimeout = 3600000; break;}
  }
  // Serial.printf("New timeout: %lu ms\r\n", actualSleepTimeout);
  resetStandbyTimer();
  // save preferences now, otherwise if you set a very big timeout and upload your firmware again, it never got saved
  save_preferences();
}

// show memory usage event handler
static void showMemoryUsage_event_cb(lv_event_t* e) {
  setShowMemoryUsage(lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED));
}

void create_tab_content_settings(lv_obj_t* tab) {

  // Add content to the settings tab
  // With a flex layout, setting groups/boxes will position themselves automatically
  lv_obj_set_layout(tab, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab, LV_SCROLLBAR_MODE_ACTIVE);

  // Add a label, then a box for the display settings -----------------------------------------
  lv_obj_t* menuLabel = lv_label_create(tab);
  lv_label_set_text(menuLabel, "Display");

  lv_obj_t* menuBox = lv_obj_create(tab);
  lv_obj_set_size(menuBox, lv_pct(100), 109);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  lv_obj_t* brightnessIcon = lv_img_create(menuBox);
  lv_img_set_src(brightnessIcon, &low_brightness);
  lv_obj_set_style_img_recolor(brightnessIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(brightnessIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(brightnessIcon, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_t* slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 60, 255);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, backlight_brightness, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(66), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 3);
  brightnessIcon = lv_img_create(menuBox);
  lv_img_set_src(brightnessIcon, &high_brightness);
  lv_obj_set_style_img_recolor(brightnessIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(brightnessIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(brightnessIcon, LV_ALIGN_TOP_RIGHT, 0, -1);
  lv_obj_add_event_cb(slider, bl_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Lift to Wake");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 32);
  lv_obj_t* wakeToggle = lv_switch_create(menuBox);
  lv_obj_set_size(wakeToggle, 40, 22);
  lv_obj_align(wakeToggle, LV_ALIGN_TOP_RIGHT, 0, 29);
  lv_obj_set_style_bg_color(wakeToggle, lv_color_hex(0x505050), LV_PART_MAIN);
  lv_obj_add_event_cb(wakeToggle, WakeEnableSetting_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  if (wakeupByIMUEnabled) {
    lv_obj_add_state(wakeToggle, LV_STATE_CHECKED);
  } else {
    // lv_obj_clear_state(wakeToggle, LV_STATE_CHECKED);
  }

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Timeout");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 64);
  lv_obj_t* drop = lv_dropdown_create(menuBox);
  lv_dropdown_set_options(drop, "10s\n"
                                "20s\n"
                                "40s\n"
                                "1m\n"
                                "3m\n"
                                "10m\n"
                                "1h"); // 1h for debug purposes, if you don't want the device to go to slepp
  // if you add more options here, do the same in timout_event_cb()
  switch (actualSleepTimeout) {
    case   10000: {lv_dropdown_set_selected(drop, 0); break;}
    case   20000: {lv_dropdown_set_selected(drop, 1); break;}
    case   40000: {lv_dropdown_set_selected(drop, 2); break;}
    case   60000: {lv_dropdown_set_selected(drop, 3); break;}
    case  180000: {lv_dropdown_set_selected(drop, 4); break;}
    case  600000: {lv_dropdown_set_selected(drop, 5); break;}
    case 3600000: {lv_dropdown_set_selected(drop, 6); break;}
  }
  lv_dropdown_set_selected_highlight(drop, true);
  lv_obj_align(drop, LV_ALIGN_TOP_RIGHT, 0, 61);
  lv_obj_set_size(drop, 70, 22);
  //lv_obj_set_style_text_font(drop, &lv_font_montserrat_12, LV_PART_MAIN);
  //lv_obj_set_style_text_font(lv_dropdown_get_list(drop), &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_set_style_pad_top(drop, 1, LV_PART_MAIN);
  lv_obj_set_style_bg_color(drop, color_primary, LV_PART_MAIN);
  lv_obj_set_style_bg_color(lv_dropdown_get_list(drop), color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(lv_dropdown_get_list(drop), 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(lv_dropdown_get_list(drop), lv_color_hex(0x505050), LV_PART_MAIN);
  lv_obj_add_event_cb(drop, timout_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // // Add another label, then a settings box for WiFi
  // menuLabel = lv_label_create(tab);
  // lv_label_set_text(menuLabel, "Wi-Fi");
  // menuBox = lv_obj_create(tab);
  // lv_obj_set_size(menuBox, lv_pct(100), 80);
  // lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  // lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);
  // menuLabel = lv_label_create(menuBox);
  // lv_label_set_text(menuLabel, "Network");
  // menuLabel = lv_label_create(menuBox);
  // lv_label_set_text(menuLabel, LV_SYMBOL_RIGHT);
  // lv_obj_align(menuLabel, LV_ALIGN_TOP_RIGHT, 0, 0);
  // menuLabel = lv_label_create(menuBox);
  // lv_label_set_text(menuLabel, "Password");
  // lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 32);
  // menuLabel = lv_label_create(menuBox);
  // lv_label_set_text(menuLabel, LV_SYMBOL_RIGHT);
  // lv_obj_align(menuLabel, LV_ALIGN_TOP_RIGHT, 0, 32);

  // Another setting for the battery ----------------------------------------------------------
  menuLabel = lv_label_create(tab);
  lv_label_set_text(menuLabel, "Battery");
  menuBox = lv_obj_create(tab);
  lv_obj_set_size(menuBox, lv_pct(100), 77); // 125
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);
  
  objBattSettingsVoltage = lv_label_create(menuBox);
  lv_label_set_text(objBattSettingsVoltage, "Voltage:");
  lv_obj_align(objBattSettingsVoltage, LV_ALIGN_TOP_LEFT, 0, 0);
  objBattSettingsPercentage = lv_label_create(menuBox);
  lv_label_set_text(objBattSettingsPercentage, "Percentage:");
  lv_obj_align(objBattSettingsPercentage, LV_ALIGN_TOP_LEFT, 0, 32);
  // objBattSettingsIscharging = lv_label_create(menuBox);
  // lv_label_set_text(objBattSettingsIscharging, "Is charging:");
  // lv_obj_align(objBattSettingsIscharging, LV_ALIGN_TOP_LEFT, 0, 64);


  // Memory statistics ------------------------------------------------------------------------
  menuLabel = lv_label_create(tab);
  lv_label_set_text(menuLabel, "Memory usage");
  menuBox = lv_obj_create(tab);
  lv_obj_set_size(menuBox, lv_pct(100), 48);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);
  
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Show mem usage");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 3);
  lv_obj_t* memoryUsageToggle = lv_switch_create(menuBox);
  lv_obj_set_size(memoryUsageToggle, 40, 22);
  lv_obj_align(memoryUsageToggle, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(memoryUsageToggle, lv_color_hex(0x505050), LV_PART_MAIN);
  lv_obj_add_event_cb(memoryUsageToggle, showMemoryUsage_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  if (getShowMemoryUsage()) {
    lv_obj_add_state(memoryUsageToggle, LV_STATE_CHECKED);
  } else {
    // lv_obj_clear_state(memoryUsageToggle, LV_STATE_CHECKED);
  }
}

void notify_tab_before_delete_settings(void) {
  // remember to set all pointers to lvgl objects to NULL if they might be accessed from outside.
  // They must check if object is NULL and must not use it if so
  objBattSettingsVoltage = NULL;
  objBattSettingsPercentage = NULL;
}

void register_gui_settings(void){
  register_gui(std::string(tabName_settings), & create_tab_content_settings, & notify_tab_before_delete_settings);
}
