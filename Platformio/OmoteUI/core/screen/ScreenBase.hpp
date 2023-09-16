#pragma once

#include "lvgl.h"

#include "UIElement.hpp"
#include "WidgetBase.hpp"

#include <vector>
namespace UI::Screen {

class Base : UIElement {
public:
  typedef std::unique_ptr<Base> Ptr;

  Base(ID aId);

  void AddWidget(Widget::Base::Ptr aWidget);

private:
  lv_obj_t *mScreen;
  std::vector<Widget::Base::Ptr> mWidgets;
};

} // namespace UI::Screen