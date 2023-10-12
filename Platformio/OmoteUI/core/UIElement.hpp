#pragma once

#include "BorderOutlinePadding.hpp"
#include "TextStyle.hpp"
#include "UIElementIds.hpp"
#include <lvgl.h>

#include "KeyPressAbstract.hpp"
#include <vector>

namespace UI {

class UIElement {

public:
  using Ptr = std::unique_ptr<UIElement>;

  UIElement(lv_obj_t *aLvglSelf, const ID aId = ID());
  virtual ~UIElement();

  virtual void SetBgColor(lv_color_t value,
                          lv_style_selector_t selector = LV_PART_MAIN);

  virtual void SetBgOpacity(lv_opa_t aOpacity,
                            lv_style_selector_t aStyle = LV_PART_MAIN);

  void SetVisiblity(bool aVisibility);
  bool IsVisible();
  bool IsSetVisible();

  virtual void SetWidth(lv_coord_t aWidth);
  virtual void SetHeight(lv_coord_t aHeight);

  lv_coord_t GetWidth();
  lv_coord_t GetHeight();

  virtual void SetContentWidth(lv_coord_t aWidth);
  virtual void SetContentHeight(lv_coord_t aHeight);

  lv_coord_t GetContentWidth();
  lv_coord_t GetContentHeight();

  virtual void SetY(lv_coord_t aY);
  virtual void SetX(lv_coord_t aX);

  lv_coord_t GetY();
  lv_coord_t GetX();
  lv_coord_t GetBottom();

  void AlignTo(UIElement *anElementToAlignWith, lv_align_t anAlignment,
               lv_coord_t aXoffset = 0, lv_coord_t aYOffset = 0);

  virtual void SetBorder(Border aNewBorder,
                         lv_style_selector_t aStyle = LV_PART_MAIN);
  Border GetBorder(lv_style_selector_t aStyle = LV_PART_MAIN);

  virtual void SetOutline(Outline aNewOutline,
                          lv_style_selector_t aStyle = LV_PART_MAIN);
  Outline GetOutline(lv_style_selector_t aStyle = LV_PART_MAIN);

  virtual void SetPadding(Padding aNewPadding,
                          lv_style_selector_t aStyle = LV_PART_MAIN);
  virtual void SetAllPadding(lv_coord_t aNewPadding,
                             lv_style_selector_t aStyle = LV_PART_MAIN);
  Padding GetPadding(lv_style_selector_t aStyle = LV_PART_MAIN);

  virtual void SetTextStyle(TextStyle aNewStyle,
                            lv_style_selector_t aStyle = LV_PART_MAIN);
  TextStyle GetTextStyle(lv_style_selector_t aStyle = LV_PART_MAIN);

  UIElement *AddElement(UIElement::Ptr anElement);
  template <class UIElemTy> UIElemTy *AddElement(UIElement::Ptr aWidget);

  UIElement::Ptr RemoveElement(UIElement *aUIElementRef);

  size_t GetNumContainedElements() { return mContainedElements.size(); }

  ID GetID() { return mId; };

  template <class UIElemTy> static UIElemTy GetElement(lv_obj_t *aLvglObject);

  /// @brief There are use cases in which objects
  ///        need to stay alive in LVGL but can die
  ///        in terms of our usage this is a helper for these
  ///        use Sparingly!!!
  /// @param aTimeToKeepLvglObj
  void SetKeepAliveTime(uint32_t aTimeToKeepLvglObj) {
    mLvglKeepAliveTime = aTimeToKeepLvglObj;
  }

  void StartLvglEventHandler();
  void StopLvglEventHandler();

protected:
  /// @brief get Lvgl object refernce to use in LVGL APIs
  /// @return lvgl object a
  lv_obj_t *LvglSelf() { return mLvglSelf; }

  /// @brief Show Element
  virtual void Show();
  /// @brief Hide Element
  virtual void Hide();
  /// @brief Override in child class to run something after element is shown
  virtual void OnShow();
  /// @brief Override in child class to run something after element is hidden
  virtual void OnHide();

  // Override in object to handle LVGL events for that object
  virtual void OnLvglEvent(lv_event_t *anEvent){};

  /// @brief Set KeyEvent to the UI element to see if it wants to handle it
  virtual bool KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent);

  /// @brief Override to Handle KeyEvent for the specific element
  /// @return true - Key event was used
  ///         fasle - Key event was unused
  virtual bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) = 0;

private:
  static void LvglEventHandler(lv_event_t *anEvent);

  lv_obj_t *mLvglSelf;
  const ID mId;
  uint32_t mLvglKeepAliveTime = 0;
  bool mIsHandlingLvglEvents = true;

  /// @brief Elements that are currently in this element
  std::vector<UIElement::Ptr> mContainedElements;
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

template <class UIElemTy>
UIElemTy *UIElement::AddElement(UIElement::Ptr anElement) {
  return static_cast<UIElemTy *>(AddElement(std::move(anElement)));
}

} // namespace UI