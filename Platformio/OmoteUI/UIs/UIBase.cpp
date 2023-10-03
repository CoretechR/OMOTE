#include "UIBase.hpp"
#include "LvglMutex.hpp"

using namespace UI;

UIBase::UIBase(std::shared_ptr<HardwareAbstract> aHardware)
    : mHardware(aHardware) {}

void UIBase::loopHandler() {
  lv_timer_handler();
  {
    auto lock = LvglMutex::lockScope();
    lv_task_handler();
  }
}