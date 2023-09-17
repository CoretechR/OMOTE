#pragma once

#include "UIElementIds.hpp"
#include "lvgl.h"

namespace UI {

class UIElement {

public:
  UIElement(lv_obj_t *aLvglSelf, const ID aId = ID());

  void SetBgColor(lv_color_t value,
                  lv_style_selector_t selector = LV_PART_MAIN);

  void SetVisiblity(bool aVisibility);
  bool IsVisible();

  lv_obj_t *operator()() { return mLvglSelf; }

protected:
  /// @brief get Lvgl object refernce to use in LVGL APIs
  /// @return lvgl object a
  lv_obj_t *LvglSelf() { return mLvglSelf; }

  /// @brief Show Element
  virtual void Show();
  /// @brief Hide Element
  virtual void Hide();
  /// @brief Override in child class to run something after element is shown
  virtual void OnShow() = 0;
  /// @brief Override in child class to run something after element is hidden
  virtual void OnHide() = 0;

private:
  lv_obj_t *mLvglSelf;
  const ID mId;
};
} // namespace UI