#include "UIElement.hpp"
#include "LvglResourceManger.hpp"

namespace UI {
UIElement::UIElement(lv_obj_t *aLvglSelf, ID aId)
    : mLvglSelf(aLvglSelf), mId(aId) {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  mLvglSelf->user_data = this;
}

UIElement::~UIElement() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  if (lv_obj_is_valid(mLvglSelf)) {
    lv_obj_del(mLvglSelf);
  }
}

void UIElement::AddElement(UIElement *anUIElement) {
  LvglResourceManger::GetInstance().AttemptNow([this, anUIElement] {
    lv_obj_set_parent(anUIElement->mLvglSelf, mLvglSelf);
  });
}

bool UIElement::IsVisible() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  return lv_obj_is_visible(mLvglSelf);
}

void UIElement::SetWidth(lv_coord_t aWidth) {
  LvglResourceManger::GetInstance().AttemptNow(
      [this, aWidth] { lv_obj_set_width(mLvglSelf, aWidth); });
}

void UIElement::SetHeight(lv_coord_t aHeight) {
  LvglResourceManger::GetInstance().AttemptNow(
      [this, aHeight] { lv_obj_set_height(mLvglSelf, aHeight); });
}

lv_coord_t UIElement::GetHeight() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_height(mLvglSelf);
};

lv_coord_t UIElement::GetWidth() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_width(mLvglSelf);
}

void UIElement::SetY(lv_coord_t aY) {
  LvglResourceManger::GetInstance().AttemptNow(
      [this, aY] { lv_obj_set_y(mLvglSelf, aY); });
};
void UIElement::SetX(lv_coord_t aX) {
  LvglResourceManger::GetInstance().AttemptNow(
      [this, aX] { lv_obj_set_x(mLvglSelf, aX); });
}

lv_coord_t UIElement::GetY() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_y(mLvglSelf);
}
lv_coord_t UIElement::GetX() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_x(mLvglSelf);
}

void UIElement::SetVisiblity(bool aVisible) {
  if (aVisible == IsVisible()) {
    return;
  }
  if (aVisible) {
    Show();
  } else {
    Hide();
  }
}

void UIElement::SetBgColor(lv_color_t aColor, lv_style_selector_t aStyle) {
  LvglResourceManger::GetInstance().AttemptNow([this, aColor, aStyle] {
    lv_obj_set_style_bg_color(mLvglSelf, aColor, aStyle);
  });
};

void UIElement::Show() {
  {
    auto lock = LvglResourceManger::GetInstance().scopeLock();
    lv_obj_clear_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN);
  }
  OnShow();
}

void UIElement::Hide() {

  {
    auto lock = LvglResourceManger::GetInstance().scopeLock();
    lv_obj_add_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN);
  }
  OnHide();
}

bool UIElement::KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return OnKeyEvent(aKeyEvent);
}

} // namespace UI
