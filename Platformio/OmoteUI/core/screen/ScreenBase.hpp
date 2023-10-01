#pragma once

#include "lvgl.h"

#include "UIElement.hpp"

#include <memory>

namespace UI::Screen {
class Manager;

class Base : public UIElement {
  friend Manager;

public:
  typedef std::unique_ptr<Base> Ptr;

  Base(ID aId);

  void SetPushAnimation(lv_scr_load_anim_t aPushAnimation);

protected:
  void Show() override;
  void OnShow() override{};
  void OnHide() override{};
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  lv_scr_load_anim_t mPushAnimation = LV_SCR_LOAD_ANIM_NONE;
};

} // namespace UI::Screen