#include <lvgl.h>
#include "device_appleTV/device_appleTV.h"
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

void init_gui_tab_appleTV(lv_obj_t* tabview) {

  lv_obj_t* tab = lv_tabview_add_tab(tabview, "Apple TV");

  // Add content to the Apple TV tab (3)
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

void init_gui_pageIndicator_appleTV(lv_obj_t* panel) {
  // Create actual (non-clickable) buttons for every tab
  lv_obj_t* btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  lv_obj_t* label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Apple TV");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

}

void register_gui_appleTV(void){
  register_gui(& init_gui_tab_appleTV, & init_gui_pageIndicator_appleTV);
}
