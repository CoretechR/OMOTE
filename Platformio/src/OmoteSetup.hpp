#pragma once
#include "HardwareFactory.hpp"
#include "UIs/HomeAssistUI/HomeAssistUI.hpp"
namespace OMOTE {
std::shared_ptr<UI::UIBase> ui = nullptr;

void setup() {
  lv_init();
  HardwareFactory::Init();
  ui = std::make_unique<UI::HomeAssistUI>();
  lv_timer_handler();  // Run the LVGL UI once before the loop takes over
}

void loop() {
  HardwareFactory::getAbstract().loopHandler();
  ui->loopHandler();
}

}  // namespace OMOTE
