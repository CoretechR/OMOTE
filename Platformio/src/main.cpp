// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include <lvgl.h>
#include "HardwareRevX.hpp"
#include "OmoteUI.hpp"
#include "omoteconfig.h"

std::shared_ptr<HardwareRevX> hal = nullptr;

void setup() {
  hal = HardwareRevX::getInstance();
  hal->init();

  auto ui = OmoteUI::getInstance(hal);
  ui->layout_UI();

  lv_timer_handler(); // Run the LVGL UI once before the loop takes over
}

void loop() {
  HardwareRevX::getInstance()->loopHandler();
  OmoteUI::getInstance()->loopHandler();
}