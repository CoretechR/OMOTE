#include "UIElement.hpp"

namespace UI {
UIElement::UIElement(lv_obj_t *aLvglSelf, ID aId)
    : mLvglSelf(aLvglSelf), mId(aId) {
  mLvglSelf->user_data = this;
}

void UIElement::AddElement(UIElement *anUIElement) {
  lv_obj_set_parent(anUIElement->mLvglSelf, mLvglSelf);
}

bool UIElement::IsVisible() { return lv_obj_is_visible(mLvglSelf); }

void UIElement::SetWidth(uint16_t aWidth) {
  lv_obj_set_width(mLvglSelf, aWidth);
}

void UIElement::SetHeight(uint16_t aHeight) {
  lv_obj_set_height(mLvglSelf, aHeight);
}

int16_t UIElement::GetHeight() { return lv_obj_get_height(mLvglSelf); };

int16_t UIElement::GetWidth() { return lv_obj_get_width(mLvglSelf); }

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

} // namespace UI
