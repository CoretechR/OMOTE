#pragma once

#include <memory>

#include "UIElement.hpp"
#include "lvgl.h"

namespace UI::Screen {
class Manager;

class Base : public UIElement {
  friend Manager;

 public:
  typedef std::unique_ptr<Base> Ptr;

  Base(ID aId);

  void SetPushAnimation(lv_screen_load_anim_t aPushAnimation);

  uint32_t GetTransitionTime();
  void SetTransitionTimes(uint32_t aAnimationTime, uint32_t aDelay = 0);

 protected:
  void Show() override;
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

 private:
  lv_screen_load_anim_t mPushAnimation = LV_SCR_LOAD_ANIM_NONE;
  uint32_t mTransitionAnimationTime = 1000;  // 1000 ms / 1 sec
  uint32_t mTransitionDelayTime = 0;
};

}  // namespace UI::Screen