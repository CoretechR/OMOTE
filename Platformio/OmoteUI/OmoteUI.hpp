// OMOTE UI
// 2023 Matthew Colvin
#pragma once

#include "HardwareInterface.h"
#include "Images.hpp"
#include "lvgl.h"
#include <algorithm>
#include <memory>
#include <stdio.h>
#include <string>


/// @brief Singleton to allow UI code to live separately from the Initialization
/// of resources.
class OmoteUI {
public:
  OmoteUI(std::shared_ptr<HardwareAbstract> aHardware)
      : mHardware(aHardware){};

  static std::weak_ptr<OmoteUI> getRefrence() { return getInstance(); };
  static std::shared_ptr<OmoteUI>
  getInstance(std::shared_ptr<HardwareAbstract> aHardware = nullptr) {
    if (mInstance) {
      return mInstance;
    } else if (aHardware) {
      mInstance = std::make_shared<OmoteUI>(aHardware);
    }
    return mInstance;
  };

  // Set the page indicator scroll position relative to the tabview scroll
  // position
  void store_scroll_value_event_cb(lv_event_t *e);
  // Update current device when the tabview page is changes
  void tabview_device_event_cb(lv_event_t *e);
  // Slider Event handler
  void bl_slider_event_cb(lv_event_t *e);
  // Apple Key Event handler
  void appleKey_event_cb(lv_event_t *e);
  // Wakeup by IMU Switch Event handler
  void WakeEnableSetting_event_cb(lv_event_t *e);
  // Smart Home Toggle Event handler
  void smartHomeToggle_event_cb(lv_event_t *e);
  // Smart Home Toggle Event handler
  void smartHomeSlider_event_cb(lv_event_t *e);
  // Virtual Keypad Event handler
  void virtualKeypad_event_cb(lv_event_t *e);

  // Use LVGL to layout the ui and register the callbacks
  void layout_UI();

  void loopHandler();

private:
  static std::shared_ptr<OmoteUI> mInstance;
  std::shared_ptr<HardwareAbstract> mHardware;

  lv_obj_t *panel = nullptr;
  Images imgs = Images();
  uint_fast8_t currentDevice = 4;
  int backlight_brightness = 255;
  lv_color_t color_primary = lv_color_hex(0x303030); // gray
  bool wakeupByIMUEnabled = true;

  inline static const uint_fast8_t virtualKeyMapTechnisat[10] = {
      0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0};
};
