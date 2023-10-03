#include "UIElement.hpp"
#include "LvglMutex.hpp"

namespace UI {
UIElement::UIElement(lv_obj_t *aLvglSelf, ID aId)
    : mLvglSelf(aLvglSelf), mId(aId) {
  auto lock = LvglMutex::lockScope();
  mLvglSelf->user_data = this;
}

UIElement::~UIElement() {
  auto lock = LvglMutex::lockScope();
  if (lv_obj_is_valid(mLvglSelf)) {
    lv_obj_del(mLvglSelf);
  }
}

void UIElement::AddElement(UIElement *anUIElement) {
  auto lock = LvglMutex::lockScope();
  lv_obj_set_parent(anUIElement->mLvglSelf, mLvglSelf);
}

bool UIElement::IsVisible() { return lv_obj_is_visible(mLvglSelf); }

void UIElement::SetWidth(lv_coord_t aWidth) {
  auto lock = LvglMutex::lockScope();
  lv_obj_set_width(mLvglSelf, aWidth);
}

void UIElement::SetHeight(lv_coord_t aHeight) {
  auto lock = LvglMutex::lockScope();
  lv_obj_set_height(mLvglSelf, aHeight);
}

lv_coord_t UIElement::GetHeight() {
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_height(mLvglSelf);
};

lv_coord_t UIElement::GetWidth() {
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_width(mLvglSelf);
}

void UIElement::SetY(lv_coord_t aY) {
  auto lock = LvglMutex::lockScope();
  lv_obj_set_y(mLvglSelf, aY);
}
void UIElement::SetX(lv_coord_t aX) {
  auto lock = LvglMutex::lockScope();
  lv_obj_set_x(mLvglSelf, aX);
}

lv_coord_t UIElement::GetY() {
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_y(mLvglSelf);
}
lv_coord_t UIElement::GetX() {
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
  auto lock = LvglMutex::lockScope();
  lv_obj_set_style_bg_color(mLvglSelf, aColor, aStyle);
};

void UIElement::Show() {
  lv_obj_clear_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN);
  OnShow();
}

void UIElement::Hide() {
  lv_obj_add_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN);
  OnHide();
}

bool UIElement::KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return OnKeyEvent(aKeyEvent);
}

} // namespace UI
