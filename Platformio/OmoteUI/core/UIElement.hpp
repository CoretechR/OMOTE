#pragma once

#include "UIElementIds.hpp"
#include "lvgl.h"

namespace UI {

class UIElement {

public:
  UIElement(lv_obj_t *aLvglSelf, const ID aId = ID());

  SetBgColor(lv_color_t value, lv_style_selector_t selector = LV_PART_MAIN);

protected:
  lv_obj_t *mLvglSelf;
  const ID mId;
};
} // namespace UI