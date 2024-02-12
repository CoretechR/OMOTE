#include <string>
#include <lvgl.h>
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/guiRegistry.h"
#include "hardware/tft.h"
#include "device_smarthome/device_smarthome.h"
#include "commandHandler.h"

// LVGL declarations
LV_IMG_DECLARE(lightbulb);

// Smart Home Toggle Event handler
static void smartHomeToggle_event_cb(lv_event_t * e){
  std::string payload;
  if (lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED)) payload = "true";
  else payload = "false";
  // Publish an MQTT message based on the event user data  
  #ifdef ENABLE_WIFI_AND_MQTT
  if((int)e->user_data == 1) executeCommand(SMARTHOME_MQTT_BULB1_SET, payload);
  if((int)e->user_data == 2) executeCommand(SMARTHOME_MQTT_BULB2_SET, payload);
  #endif
}

// Smart Home Slider Event handler
static void smartHomeSlider_event_cb(lv_event_t * e){
  lv_obj_t * slider = lv_event_get_target(e);
  char payload[8];
  dtostrf(lv_slider_get_value(slider), 1, 2, payload);
  std::string payload_str(payload);
  // Publish an MQTT message based on the event user data
  #ifdef ENABLE_WIFI_AND_MQTT
  if((int)e->user_data == 1) executeCommand(SMARTHOME_MQTT_BULB1_BRIGHTNESS_SET, payload);
  if((int)e->user_data == 2) executeCommand(SMARTHOME_MQTT_BULB2_BRIGHTNESS_SET, payload);
  #endif
}

void init_gui_tab_smarthome(lv_obj_t* tabview) {

  lv_obj_t* tab = lv_tabview_add_tab(tabview, "Smart Home");

  // Add content to the smart home tab (4)
  lv_obj_set_layout(tab, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab, LV_SCROLLBAR_MODE_ACTIVE);

  // Add a label, then a box for the light controls
  lv_obj_t* menuLabel = lv_label_create(tab);
  lv_label_set_text(menuLabel, "Living Room");

  lv_obj_t* menuBox = lv_obj_create(tab);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  lv_obj_t* bulbIcon = lv_img_create(menuBox);
  lv_img_set_src(bulbIcon, &lightbulb);
  lv_obj_set_style_img_recolor(bulbIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(bulbIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(bulbIcon, LV_ALIGN_TOP_LEFT, 0, 0);

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Floor Lamp");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 22, 3);
  lv_obj_t* lightToggleA = lv_switch_create(menuBox);
  lv_obj_set_size(lightToggleA, 40, 22);
  lv_obj_align(lightToggleA, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(lightToggleA, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleA, color_primary, LV_PART_INDICATOR);
  lv_obj_add_event_cb(lightToggleA, smartHomeToggle_event_cb, LV_EVENT_VALUE_CHANGED, (void*)1);

  lv_obj_t* slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 0, 100);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(slider, smartHomeSlider_event_cb, LV_EVENT_VALUE_CHANGED, (void*)1);

  // Add another menu box for a second appliance
  menuBox = lv_obj_create(tab);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  bulbIcon = lv_img_create(menuBox);
  lv_img_set_src(bulbIcon, &lightbulb);
  lv_obj_set_style_img_recolor(bulbIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(bulbIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(bulbIcon, LV_ALIGN_TOP_LEFT, 0, 0);

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Ceiling Light");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 22, 3);
  lv_obj_t* lightToggleB = lv_switch_create(menuBox);
  lv_obj_set_size(lightToggleB, 40, 22);
  lv_obj_align(lightToggleB, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(lightToggleB, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleB, color_primary, LV_PART_INDICATOR);
  lv_obj_add_event_cb(lightToggleB, smartHomeToggle_event_cb, LV_EVENT_VALUE_CHANGED, (void*)2);

  slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 0, 100);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(slider, smartHomeSlider_event_cb, LV_EVENT_VALUE_CHANGED, (void*)2);


  // Add another room (empty for now)
  menuLabel = lv_label_create(tab);
  lv_label_set_text(menuLabel, "Kitchen");

  menuBox = lv_obj_create(tab);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

}

void init_gui_pageIndicator_smarthome(lv_obj_t* panel) {
  // Create actual (non-clickable) buttons for every tab
  lv_obj_t* btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  lv_obj_t* label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Smart Home");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

}

void register_gui_smarthome(void){
  register_gui(& init_gui_tab_smarthome, & init_gui_pageIndicator_smarthome);
}
