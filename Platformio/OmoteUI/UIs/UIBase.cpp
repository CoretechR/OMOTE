#include "UIBase.hpp"

using namespace UI;

UIBase::UIBase(std::shared_ptr<HardwareAbstract> aHardware)
    : mHardware(aHardware) {}

void UIBase::loopHandler() {
  lv_timer_handler();
  lv_task_handler();
}