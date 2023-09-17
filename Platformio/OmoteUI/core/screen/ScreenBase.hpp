#pragma once

#include "lvgl.h"

#include "UIElement.hpp"
#include "WidgetBase.hpp"

#include <vector>
namespace UI::Screen {
class Manager;

class Base : public UIElement {
  friend Manager;

public:
  typedef std::unique_ptr<Base> Ptr;

  Base(ID aId);

  void AddWidget(Widget::Base::Ptr aWidget);
  void SetPushAnimation(lv_scr_load_anim_t aPushAnimation);

protected:
  void Show() override;
  void OnShow() override{};
  void OnHide() override{};

private:
  std::vector<Widget::Base::Ptr> mWidgets;
  lv_scr_load_anim_t mPushAnimation = LV_SCR_LOAD_ANIM_NONE;
};

} // namespace UI::Screen