#include "UIBase.hpp"
#include "LvglResourceManger.hpp"

using namespace UI;

UIBase::UIBase(std::shared_ptr<HardwareAbstract> aHardware)
    : mHardware(aHardware) {}

void UIBase::loopHandler() {
  {
    auto lock = LvglResourceManger::GetInstance().scopeLock();
    lv_timer_handler();
    lv_task_handler();
  }
  LvglResourceManger::GetInstance().HandleQueuedTasks();
}