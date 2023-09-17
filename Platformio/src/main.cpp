// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include "HardwareRevX.hpp"
#include "OmoteUI.hpp"
#include "omoteconfig.h"
#include <lvgl.h>

std::shared_ptr<HardwareRevX> hal = nullptr;

void setup() {
  hal = HardwareRevX::getInstance();
  hal->init();

  auto ui = UI::Basic::OmoteUI::getInstance(hal);
  ui->layout_UI();

  lv_timer_handler(); // Run the LVGL UI once before the loop takes over
}

void loop() {
  HardwareRevX::getInstance()->loopHandler();
  UI::Basic::OmoteUI::getInstance()->loopHandler();
}