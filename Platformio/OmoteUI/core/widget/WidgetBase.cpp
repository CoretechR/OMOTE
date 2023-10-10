#include "WidgetBase.hpp"

using namespace UI::Widget;

Base::Base(lv_obj_t *aLvglSelf) : UIElement(aLvglSelf) {
  SetWidth(lv_pct(100));
  SetHeight(lv_pct(100));
}
