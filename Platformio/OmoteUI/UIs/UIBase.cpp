#include "UIBase.hpp"
#include "LvglResourceManager.hpp"

using namespace UI;

UIBase::UIBase(std::shared_ptr<HardwareAbstract> aHardware)
    : mHardware(aHardware) {}

void UIBase::loopHandler() {
  {
    auto lock = LvglResourceManager::GetInstance().scopeLock();
    lv_timer_handler();
    lv_task_handler();
  }
  LvglResourceManager::GetInstance().HandleQueuedTasks();
}