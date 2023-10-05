// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include "BasicUI.hpp"
#include "HardwareRevX.hpp"
#include "omoteconfig.h"
#include <lvgl.h>

std::shared_ptr<HardwareRevX> hal = nullptr;
std::shared_ptr<UI::UIBase> ui = nullptr;

void setup() {
  hal = HardwareRevX::getInstance();
  hal->init();

  auto ui = UI::BasicUI(hal);
  // ui->layout_UI();

  lv_timer_handler(); // Run the LVGL UI once before the loop takes over
}

void loop() {
  hal->loopHandler();
  ui->loopHandler();
}