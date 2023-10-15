#include "BasicUI.hpp"
#include "HardwareFactory.hpp"

namespace OMOTE {
std::shared_ptr<UI::UIBase> ui = nullptr;

void setup() {
  HardwareFactory::getAbstract().init();
  ui = std::make_unique<UI::BasicUI>();
  lv_timer_handler(); // Run the LVGL UI once before the loop takes over
}

void loop() {
  HardwareFactory::getAbstract().loopHandler();
  ui->loopHandler();
}

} // namespace OMOTE
