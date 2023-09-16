#pragma once

#include "UIElementIds.hpp"
#include "lvgl.h"

namespace UI {

class UIElement {

public:
  UIElement(lv_obj_t *aLvglSelf, const ID aId = ID());

protected:
  lv_obj_t *mLvglSelf;
  const ID mId;
};
} // namespace UI