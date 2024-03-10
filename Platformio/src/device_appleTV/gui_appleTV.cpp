#include <lvgl.h>
#include "device_appleTV/device_appleTV.h"
#include "device_appleTV/gui_appleTV.h"
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/guiRegistry.h"
#include "hardware/tft.h"
#include "commandHandler.h"

// LVGL declarations
LV_IMG_DECLARE(appleTvIcon);
LV_IMG_DECLARE(appleDisplayIcon);
LV_IMG_DECLARE(appleBackIcon);

// Apple Key Event handler
static void appleKey_event_cb(lv_event_t* e) {
  // Send IR command based on the event user data  
  executeCommand(APPLETV_GUI_EVENT_USER_DATA, std::to_string(50 + (int)e->user_data));
  Serial.println(50 + (int)e->user_data);
}

void create_tab_content_appleTV(lv_obj_t* tab) {

  // Add content to the Apple TV tab
  // Add a nice apple tv logo
  lv_obj_t* appleImg = lv_img_create(tab);
  lv_img_set_src(appleImg, &appleTvIcon);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, -60);
  // create two buttons and add their icons accordingly
  lv_obj_t* button = lv_btn_create(tab);
  lv_obj_align(button, LV_ALIGN_BOTTOM_LEFT, 10, 0);
  lv_obj_set_size(button, 60, 60);
  lv_obj_set_style_radius(button, 30, LV_PART_MAIN);
  lv_obj_set_style_bg_color(button, color_primary, LV_PART_MAIN);
  lv_obj_add_event_cb(button, appleKey_event_cb, LV_EVENT_CLICKED, (void*)1);

  appleImg = lv_img_create(button);
  lv_img_set_src(appleImg, &appleBackIcon);
  lv_obj_set_style_img_recolor(appleImg, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(appleImg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, -3, 0);

  button = lv_btn_create(tab);
  lv_obj_align(button, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
  lv_obj_set_size(button, 60, 60);
  lv_obj_set_style_radius(button, 30, LV_PART_MAIN);
  lv_obj_set_style_bg_color(button, color_primary, LV_PART_MAIN);
  lv_obj_add_event_cb(button, appleKey_event_cb, LV_EVENT_CLICKED, (void*)2);

  appleImg = lv_img_create(button);
  lv_img_set_src(appleImg, &appleDisplayIcon);
  lv_obj_set_style_img_recolor(appleImg, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(appleImg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, 0);

}

void notify_tab_before_delete_appleTV(void) {
  // remember to set all pointers to lvgl objects to NULL if they might be accessed from outside.
  // They must check if object is NULL and must not use it if so

}

void register_gui_appleTV(void){
  register_gui(std::string(tabName_appleTV), & create_tab_content_appleTV, & notify_tab_before_delete_appleTV);
}
