#include "UIBase.hpp"
#include "LvglResourceManager.hpp"

using namespace UI;

UIBase::UIBase() {}

void UIBase::loopHandler() {
  {
    auto lock = LvglResourceManager::GetInstance().scopeLock();
    lv_timer_handler();
    lv_task_handler();
  }
  LvglResourceManager::GetInstance().HandleQueuedTasks();
}