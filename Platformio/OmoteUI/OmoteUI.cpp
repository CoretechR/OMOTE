#include "OmoteUI.hpp"
#include "lvgl.h"
#include "omoteconfig.h"
#include <functional>

std::shared_ptr<OmoteUI> OmoteUI::mInstance = nullptr;

// This can be used to flag out specific code for SIM only
// #if defined(IS_SIMULATOR) && (IS_SIMULATOR == true)
// #endif

// Set the page indicator scroll position relative to the tabview scroll
// position
void OmoteUI::store_scroll_value_event_cb(lv_event_t *e) {
  float bias = (150.0 + 8.0) / 240.0;
  int offset = 240 / 2 - 150 / 2 - 8 - 50 - 3;
  lv_obj_t *screen = lv_event_get_target(e);
  lv_obj_scroll_to_x(panel, lv_obj_get_scroll_x(screen) * bias - offset,
                     LV_ANIM_OFF);
}

// Update current device when the tabview page is changes
void OmoteUI::tabview_device_event_cb(lv_event_t *e) {
  currentDevice = lv_tabview_get_tab_act(lv_event_get_target(e));
}

// Slider Event handler
void OmoteUI::bl_slider_event_cb(lv_event_t *e) {
  lv_obj_t *slider = lv_event_get_target(e);
  backlight_brightness = std::clamp(lv_slider_get_value(slider), 60, 255);
}

// Apple Key Event handler
void OmoteUI::appleKey_event_cb(lv_event_t *e) {
  // Send IR command based on the event user data
  //mHardware->debugPrint(std::to_string(50 + (int)e->user_data));
}

// Wakeup by IMU Switch Event handler
void OmoteUI::WakeEnableSetting_event_cb(lv_event_t *e) {
  wakeupByIMUEnabled =
      lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
}

// Smart Home Toggle Event handler
void OmoteUI::smartHomeToggle_event_cb(lv_event_t *e) {
  char payload[8];
  if (lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED))
    strcpy(payload, "true");
  else
    strcpy(payload, "false");
  // Publish an MQTT message based on the event user data
  // if ((int)e->user_data == 1)
  //   mHardware->MQTTPublish("bulb1_set", payload);
  // if ((int)e->user_data == 2)
  //   mHardware->MQTTPublish("bulb2_set", payload);
}

// Smart Home Toggle Event handler
void OmoteUI::smartHomeSlider_event_cb(lv_event_t *e) {
  lv_obj_t *slider = lv_event_get_target(e);
  char payload[8];
  auto sliderValue = lv_slider_get_value(slider);

  // TODO convert this dtostrf to somethign more portable.
  //  I gave it a stab here but not sure it is the same.
  // dtostrf(lv_slider_get_value(slider), 1, 2, payload);
  snprintf(payload, sizeof(payload), "%8.2f", sliderValue);

  // Publish an MQTT message based on the event user data
  // if ((int)e->user_data == 1)
  //   mHardware->MQTTPublish("bulb1_setbrightness", payload);
  // if ((int)e->user_data == 2)
  //   mHardware->MQTTPublish("bulb2_setbrightness", payload);
}

void OmoteUI::virtualKeypad_event_cb(lv_event_t *e) {
  lv_obj_t *target = lv_event_get_target(e);
  lv_obj_t *cont = lv_event_get_current_target(e);
  if (target == cont)
    return;

  char buffer[100];
  // sprintf(buffer, "check it out: %d\n",
  //         virtualKeyMapTechnisat[(int)target->user_data]);
  // mHardware->debugPrint(buffer);
}

void OmoteUI::loopHandler(){
  lv_timer_handler();
}

void OmoteUI::layout_UI() {

  // --- LVGL UI Configuration ---

  // Set the background color
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);

  // Setup a scrollable tabview for devices and settings
  lv_obj_t *tabview;
  tabview =
      lv_tabview_create(lv_scr_act(), LV_DIR_TOP,
                        0); // Hide tab labels by setting their height to 0
  lv_obj_set_style_bg_color(tabview, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_size(tabview, SCREEN_WIDTH,
                  270); // 270 = screenHeight(320) - panel(30) - statusbar(20)
  lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);

  // Add 4 tabs (names are irrelevant since the labels are hidden)
  lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Settings");
  lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Technisat");
  lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Apple TV");
  lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "Smart Home");

  // Configure number button grid
  static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
                                 LV_GRID_TEMPLATE_LAST}; // equal x distribution
  static lv_coord_t row_dsc[] = {
      52, 52, 52, 52, LV_GRID_TEMPLATE_LAST}; // manual y distribution to
                                              // compress the grid a bit

  // Create a container with grid for tab2
  lv_obj_set_style_pad_all(tab2, 0, LV_PART_MAIN);
  lv_obj_t *cont = lv_obj_create(tab2);
  lv_obj_set_style_shadow_width(cont, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(cont, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
  lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
  lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
  lv_obj_set_size(cont, 240, 270);
  lv_obj_set_layout(cont, LV_LAYOUT_GRID);
  lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_radius(cont, 0, LV_PART_MAIN);

  lv_obj_t *buttonLabel;
  lv_obj_t *obj;

  // Iterate through grid buttons and configure them
  for (int i = 0; i < 12; i++) {
    uint8_t col = i % 3;
    uint8_t row = i / 3;
    // Create the button object
    if ((row == 3) && ((col == 0) || (col == 2)))
      continue; // Do not create a complete fourth row, only a 0 button
    obj = lv_btn_create(cont);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                         LV_GRID_ALIGN_STRETCH, row, 1);
    lv_obj_set_style_bg_color(obj, color_primary, LV_PART_MAIN);
    lv_obj_set_style_radius(obj, 14, LV_PART_MAIN);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE); // Clicking a button causes
                                                    // a event in its container
    // Create Labels for each button
    buttonLabel = lv_label_create(obj);
    if (i < 9) {
      lv_label_set_text_fmt(buttonLabel, std::to_string(i + 1).c_str(), col,
                            row);
      lv_obj_set_user_data(obj,
                           (void *)i); // Add user data so we can identify which
                                       // button caused the container event
    } else {
      lv_label_set_text_fmt(buttonLabel, "0", col, row);
      lv_obj_set_user_data(obj, (void *)9);
    }
    lv_obj_set_style_text_font(buttonLabel, &lv_font_montserrat_24,
                               LV_PART_MAIN);
    lv_obj_center(buttonLabel);
  }
  // Create a shared event for all button inside container
  lv_obj_add_event_cb(
      cont, [](lv_event_t *e) { mInstance->virtualKeypad_event_cb(e); },
      LV_EVENT_CLICKED, NULL);

  // Add content to the Apple TV tab (3)
  // Add a nice apple tv logo

  lv_obj_t* appleImg = imgs.addAppleTVIcon(tab3);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, -60);

  // create two buttons and add their icons accordingly
  lv_obj_t *button = lv_btn_create(tab3);
  lv_obj_align(button, LV_ALIGN_BOTTOM_LEFT, 10, 0);
  lv_obj_set_size(button, 60, 60);
  lv_obj_set_style_radius(button, 30, LV_PART_MAIN);
  lv_obj_set_style_bg_color(button, color_primary, LV_PART_MAIN);
  lv_obj_add_event_cb(
      button, [](lv_event_t *e) { mInstance->appleKey_event_cb(e); },
      LV_EVENT_CLICKED, (void *)1);

  appleImg = imgs.addAppleBackIcon(button);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, -3, 0);
  lv_obj_set_style_img_recolor(appleImg, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(appleImg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, -3, 0);

  button = lv_btn_create(tab3);
  lv_obj_align(button, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
  lv_obj_set_size(button, 60, 60);
  lv_obj_set_style_radius(button, 30, LV_PART_MAIN);
  lv_obj_set_style_bg_color(button, color_primary, LV_PART_MAIN);
  lv_obj_add_event_cb(
      button, [](lv_event_t *e) { mInstance->appleKey_event_cb(e); },
      LV_EVENT_CLICKED, (void *)2);

  appleImg = imgs.addAppleDisplayImage(button);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_img_recolor(appleImg, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(appleImg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, 0);

  // Add content to the settings tab
  // With a flex layout, setting groups/boxes will position themselves
  // automatically
  lv_obj_set_layout(tab1, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab1, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_ACTIVE);


  // Add a label, then a box for the display settings
  lv_obj_t *menuLabel = lv_label_create(tab1);
  lv_label_set_text(menuLabel, "Display");

  lv_obj_t *menuBox = lv_obj_create(tab1);
  lv_obj_set_size(menuBox, lv_pct(100), 109);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  lv_obj_t *brightnessIcon = imgs.addLowBrightnessIcon(menuBox);
  lv_obj_align(brightnessIcon, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 60, 255);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50),
                            LV_PART_MAIN);
  lv_slider_set_value(slider, backlight_brightness, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(66), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 3);

  brightnessIcon = imgs.addHighBrightnessIcon(menuBox);
  lv_obj_align(brightnessIcon, LV_ALIGN_TOP_RIGHT, 0, -1);

  lv_obj_add_event_cb(
      slider, [](lv_event_t *e) { mInstance->bl_slider_event_cb(e); },
      LV_EVENT_VALUE_CHANGED, NULL);

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Lift to Wake");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 32);
  lv_obj_t *wakeToggle = lv_switch_create(menuBox);
  lv_obj_set_size(wakeToggle, 40, 22);
  lv_obj_align(wakeToggle, LV_ALIGN_TOP_RIGHT, 0, 29);
  lv_obj_set_style_bg_color(wakeToggle, lv_color_lighten(color_primary, 50),
                            LV_PART_MAIN);
  lv_obj_add_event_cb(
      wakeToggle,
      [](lv_event_t *e) { mInstance->WakeEnableSetting_event_cb(e); },
      LV_EVENT_VALUE_CHANGED, NULL);
  if (wakeupByIMUEnabled)
    lv_obj_add_state(wakeToggle, LV_STATE_CHECKED); // set default state

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Timeout");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 64);
  lv_obj_t *drop = lv_dropdown_create(menuBox);
  lv_dropdown_set_options(drop, "10s\n"
                                "30s\n"
                                "1m\n"
                                "3m");
  lv_obj_align(drop, LV_ALIGN_TOP_RIGHT, 0, 61);
  lv_obj_set_size(drop, 70, 22);
  lv_obj_set_style_pad_top(drop, 1, LV_PART_MAIN);
  lv_obj_set_style_bg_color(drop, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(drop, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(lv_dropdown_get_list(drop), color_primary,
                            LV_PART_MAIN);
  lv_obj_set_style_border_width(lv_dropdown_get_list(drop), 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(lv_dropdown_get_list(drop),
                                lv_color_darken(color_primary, 40),
                                LV_PART_MAIN);

  // Add another label, then a settings box for WiFi
  menuLabel = lv_label_create(tab1);
  lv_label_set_text(menuLabel, "Wi-Fi");
  menuBox = lv_obj_create(tab1);
  lv_obj_set_size(menuBox, lv_pct(100), 80);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Network");
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, LV_SYMBOL_RIGHT);
  lv_obj_align(menuLabel, LV_ALIGN_TOP_RIGHT, 0, 0);
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Password");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 32);
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, LV_SYMBOL_RIGHT);
  lv_obj_align(menuLabel, LV_ALIGN_TOP_RIGHT, 0, 32);

  // Another setting for the battery
  menuLabel = lv_label_create(tab1);
  lv_label_set_text(menuLabel, "Battery");
  menuBox = lv_obj_create(tab1);
  lv_obj_set_size(menuBox, lv_pct(100), 125);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  mHardware->onBatteryChange([menuLabel](HardwareAbstract::batteryStatus aCurrentBattery){
    // I dont know enough about lvgl to do this but basically take aCurrentBattery and update UI elements here.
    // See Notice menuLabel is captured and useable here.
  });

  // Add content to the smart home tab (4)

  lv_obj_set_layout(tab4, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab4, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab4, LV_SCROLLBAR_MODE_ACTIVE);

  // Add a label, then a box for the light controls
  menuLabel = lv_label_create(tab4);
  lv_label_set_text(menuLabel, "Living Room");

  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  lv_obj_t *bulbIcon = imgs.addLightBulbIcon(menuBox);
  lv_obj_align(bulbIcon, LV_ALIGN_TOP_LEFT, 0, 0);

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Floor Lamp");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 22, 3);
  lv_obj_t *lightToggleA = lv_switch_create(menuBox);
  lv_obj_set_size(lightToggleA, 40, 22);
  lv_obj_align(lightToggleA, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(lightToggleA, lv_color_lighten(color_primary, 50),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleA, color_primary, LV_PART_INDICATOR);
  lv_obj_add_event_cb(
      lightToggleA,
      [](lv_event_t *e) { mInstance->smartHomeToggle_event_cb(e); },
      LV_EVENT_VALUE_CHANGED, (void *)1);

  slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 60, 255);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30),
                            LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(
      slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180),
      LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50),
                            LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(
      slider, [](lv_event_t *e) { mInstance->smartHomeSlider_event_cb(e); },
      LV_EVENT_VALUE_CHANGED, (void *)1);

  // Add another menu box for a second appliance
  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  bulbIcon = imgs.addLightBulbIcon(menuBox);
  lv_obj_align(bulbIcon, LV_ALIGN_TOP_LEFT, 0, 0);

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Ceiling Light");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 22, 3);
  lv_obj_t *lightToggleB = lv_switch_create(menuBox);
  lv_obj_set_size(lightToggleB, 40, 22);
  lv_obj_align(lightToggleB, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(lightToggleB, lv_color_lighten(color_primary, 50),
                            LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleB, color_primary, LV_PART_INDICATOR);
  lv_obj_add_event_cb(
      lightToggleB,
      [](lv_event_t *e) { mInstance->smartHomeToggle_event_cb(e); },
      LV_EVENT_VALUE_CHANGED, (void *)2);

  slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 60, 255);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30),
                            LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(
      slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180),
      LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50),
                            LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(
      slider, [](lv_event_t *e) { mInstance->smartHomeSlider_event_cb(e); },
      LV_EVENT_VALUE_CHANGED, (void *)2);

  // Add another room (empty for now)
  menuLabel = lv_label_create(tab4);
  lv_label_set_text(menuLabel, "Kitchen");

  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  // Set current page according to the current Device
  lv_tabview_set_act(tabview, currentDevice, LV_ANIM_OFF);

  // Create a page indicator
  panel = lv_obj_create(lv_scr_act());
  lv_obj_clear_flag(
      panel, LV_OBJ_FLAG_CLICKABLE); // this indicator will not be clickable
  lv_obj_set_size(panel, SCREEN_WIDTH, 30);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
  lv_obj_align(panel, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
  // This small hidden button enables the page indicator to scroll further
  lv_obj_t *btn = lv_btn_create(panel);
  lv_obj_set_size(btn, 50, lv_pct(100));
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
  // Create actual (non-clickable) buttons for every tab
  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Settings");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Technisat");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Apple TV");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Smart Home");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);
  // This small hidden button enables the page indicator to scroll further
  btn = lv_btn_create(panel);
  lv_obj_set_size(btn, 50, lv_pct(100));
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);

  // Make the indicator scroll together with the tabs by creating a scroll event
  lv_obj_add_event_cb(
      lv_tabview_get_content(tabview),
      [](lv_event_t *e) { mInstance->store_scroll_value_event_cb(e); },
      LV_EVENT_SCROLL, NULL);
  lv_obj_add_event_cb(
      tabview, [](lv_event_t *e) { mInstance->tabview_device_event_cb(e); },
      LV_EVENT_VALUE_CHANGED, NULL);
  // Initialize scroll position for the indicator
  lv_event_send(lv_tabview_get_content(tabview), LV_EVENT_SCROLL, NULL);

  // Style the panel background
  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_pad_all(&style_btn, 3);
  lv_style_set_border_width(&style_btn, 0);
  lv_style_set_bg_opa(&style_btn, LV_OPA_TRANSP);
  lv_obj_add_style(panel, &style_btn, 0);

  // Make the indicator fade out at the sides using gradient bitmaps
  lv_obj_t *img1 = imgs.addLeftGradiant(lv_scr_act());
  lv_obj_align(img1, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_set_size(img1, 30, 30); // stretch the 1-pixel high image to 30px

  lv_obj_t* img2 = imgs.addRightGradiant(lv_scr_act());
  lv_obj_align(img2, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_size(img2, 30, 30);

  // Create a status bar
  lv_obj_t *statusbar = lv_btn_create(lv_scr_act());
  lv_obj_set_size(statusbar, 240, 20);
  lv_obj_set_style_shadow_width(statusbar, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(statusbar, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
  lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, 0);

  lv_obj_t *WifiLabel = lv_label_create(statusbar);
  lv_label_set_text(WifiLabel, LV_SYMBOL_WIFI);
  lv_obj_align(WifiLabel, LV_ALIGN_LEFT_MID, -8, 0);
  lv_obj_set_style_text_font(WifiLabel, &lv_font_montserrat_14, LV_PART_MAIN);

  lv_obj_t *objBattPercentage = lv_label_create(statusbar);
  lv_label_set_text(objBattPercentage, "");
  lv_obj_align(objBattPercentage, LV_ALIGN_RIGHT_MID, -16, 0);
  lv_obj_set_style_text_font(objBattPercentage, &lv_font_montserrat_14,
                             LV_PART_MAIN);

  lv_obj_t *objBattIcon = lv_label_create(statusbar);
  lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
  lv_obj_align(objBattIcon, LV_ALIGN_RIGHT_MID, 8, 0);
  lv_obj_set_style_text_font(objBattIcon, &lv_font_montserrat_14, LV_PART_MAIN);
}
