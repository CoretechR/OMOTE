#pragma once

#include "UIElementIds.hpp"
#include "lvgl.h"

namespace UI {

class UIElement {

public:
  UIElement(lv_obj_t *aLvglSelf, const ID aId = ID());

  virtual void SetBgColor(lv_color_t value,
                          lv_style_selector_t selector = LV_PART_MAIN);

  void SetVisiblity(bool aVisibility);
  bool IsVisible();

  virtual void SetWidth(uint16_t aWidth);
  virtual void SetHeight(uint16_t aHeight);

  int16_t GetWidth();
  int16_t GetHeight();

  virtual void AddElement(UIElement *anElement);

  ID GetID() { return mId; };

  template <class UIElemTy> static UIElemTy GetElement(lv_obj_t *aLvglObject);

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

/**
 * @brief This helper allows conversion between anLvglObject and a
 *        core element by using the user data that links the LVGL object
 *        to its C++ counterpart. Do note that it is possible that this
 *        user data is not always there if the Lvgl Object has not been wrapped
 *        by UIElement
 *
 * @tparam UIElemTy - Type of element you want to cast to
 * @param aLvglObject - object to extract User data from
 * @return UIElemTy - object stored in user data (See constructor of
 * UIElement)
 */
template <class UIElemTy>
UIElemTy UIElement::GetElement(lv_obj_t *aLvglObject) {
  auto UIElement = lv_obj_get_user_data(aLvglObject);
  if (UIElement) {
    return static_cast<UIElemTy>(UIElement);
  }
  return nullptr;
}

} // namespace UI