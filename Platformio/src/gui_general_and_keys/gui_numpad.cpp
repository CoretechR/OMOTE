#include <lvgl.h>
#include "hardware/tft.h"
#include "device_samsungTV/device_samsungTV.h"
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/guiRegistry.h"
#include "gui_general_and_keys/gui_numpad.h"
#include "commandHandler.h"
#include "scenes/sceneHandler.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"

// Virtual Keypad Event handler
static void virtualKeypad_event_cb(lv_event_t* e) {
  lv_obj_t* target = lv_event_get_target(e);
  lv_obj_t* cont = lv_event_get_current_target(e);
  if (target == cont) return; // stop if container was clicked
  
  // send corrensponding number
  if (currentScene == scene_name_TV) {
    std::string virtualKeyMapTVNumbers[10] = {SAMSUNG_NUM_1, SAMSUNG_NUM_2, SAMSUNG_NUM_3, SAMSUNG_NUM_4, SAMSUNG_NUM_5, SAMSUNG_NUM_6, SAMSUNG_NUM_7, SAMSUNG_NUM_8, SAMSUNG_NUM_9, SAMSUNG_NUM_0};
    std::string command = virtualKeyMapTVNumbers[(int)target->user_data];
    executeCommand(command);

  } else if (currentScene == scene_name_fireTV) {
    byte virtualKeyMapFireTVNumbers[10] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0};
    int number = virtualKeyMapFireTVNumbers[(int)target->user_data];
    std::string numberStr = std::to_string(number);
    executeCommand(KEYBOARD_SENDSTRING, numberStr);
  }
}

void create_tab_content_numpad(lv_obj_t* tab) {

  // Configure number button grid 
  static lv_coord_t col_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST }; // equal x distribution
  static lv_coord_t row_dsc[] = { 52, 52, 52, 52, LV_GRID_TEMPLATE_LAST }; // manual y distribution to compress the grid a bit

  // Create a container with grid for tab
  lv_obj_set_style_pad_all(tab, 0, LV_PART_MAIN);
  lv_obj_t* cont = lv_obj_create(tab);
  lv_obj_set_style_shadow_width(cont, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(cont, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
  lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
  lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
  lv_obj_set_size(cont, 240, 270);
  lv_obj_set_layout(cont, LV_LAYOUT_GRID);
  lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_radius(cont, 0, LV_PART_MAIN);

  lv_obj_t* buttonLabel;
  lv_obj_t* obj;

  // Iterate through grid buttons configure them
  for (int i = 0; i < 12; i++) {
    uint8_t col = i % 3;
    uint8_t row = i / 3;
    // Create the button object
    if ((row == 3) && ((col == 0) || (col == 2))) continue; // Do not create a complete fourth row, only a 0 button
    obj = lv_btn_create(cont);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
    lv_obj_set_style_bg_color(obj, color_primary, LV_PART_MAIN);
    lv_obj_set_style_radius(obj, 14, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(obj, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE); // Clicking a button causes a event in its container
    // Create Labels for each button
    buttonLabel = lv_label_create(obj);        
    if(i < 9){
      lv_label_set_text_fmt(buttonLabel, std::to_string(i+1).c_str(), col, row);
      lv_obj_set_user_data(obj, (void*)i); // Add user data so we can identify which button caused the container event
    }
    else{
      lv_label_set_text_fmt(buttonLabel, "0", col, row);
      lv_obj_set_user_data(obj, (void*)9);
    } 
    lv_obj_set_style_text_font(buttonLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_center(buttonLabel);
  }
  // Create a shared event for all button inside container
  lv_obj_add_event_cb(cont, virtualKeypad_event_cb, LV_EVENT_CLICKED, NULL);

}

void notify_tab_before_delete_numpad(void) {
  // remember to set all pointers to lvgl objects to NULL if they might be accessed from outside.
  // They must check if object is NULL and must not use it if so

}

void register_gui_numpad(void){
  register_gui(std::string(tabName_numpad), & create_tab_content_numpad, & notify_tab_before_delete_numpad);
}
