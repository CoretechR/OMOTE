#include "UIElement.hpp"
#include "LvglResourceManger.hpp"

namespace UI {
UIElement::UIElement(lv_obj_t *aLvglSelf, ID aId)
    : mLvglSelf(aLvglSelf), mId(aId) {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  mLvglSelf->user_data = this;
  // Register Handler so that all object are able to override OnLvglEvent to
  // handle events easilyOnLvEvent
  lv_obj_add_event_cb(mLvglSelf, UIElement::LvglEventHandler, LV_EVENT_ALL,
                      this);
}

UIElement::~UIElement() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  if (lv_obj_is_valid(LvglSelf())) {
    if (mLvglKeepAliveTime > 0) {
      lv_obj_del_delayed(LvglSelf(), mLvglKeepAliveTime);
    } else {
      lv_obj_del(LvglSelf());
    }
    lv_obj_remove_event_cb(mLvglSelf, UIElement::LvglEventHandler);
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

void UIElement::SetContentWidth(lv_coord_t aWidth) {
  LvglResourceManger::GetInstance().AttemptNow(
      [this, aWidth] { lv_obj_set_content_width(mLvglSelf, aWidth); });
}
void UIElement::SetContentHeight(lv_coord_t aHeight) {
  LvglResourceManger::GetInstance().AttemptNow(
      [this, aHeight] { lv_obj_set_content_height(mLvglSelf, aHeight); });
}

lv_coord_t UIElement::GetContentWidth() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  return lv_obj_get_content_width(mLvglSelf);
}
lv_coord_t UIElement::GetContentHeight() {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  return lv_obj_get_content_height(mLvglSelf);
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

lv_coord_t UIElement::GetBottom() { return GetY() + GetHeight(); };

void UIElement::SetBorder(Border aNewBorder, lv_style_selector_t aStyle) {
  LvglResourceManger::GetInstance().AttemptNow([this, aNewBorder, aStyle] {
    lv_obj_set_style_border_color(mLvglSelf, aNewBorder.color, aStyle);
    lv_obj_set_style_border_opa(mLvglSelf, aNewBorder.opacity, aStyle);
    lv_obj_set_style_border_side(mLvglSelf, aNewBorder.sides, aStyle);
    lv_obj_set_style_border_width(mLvglSelf, aNewBorder.width, aStyle);
  });
}

Border UIElement::GetBorder(lv_style_selector_t aStyle) {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  return Border()
      .Color(lv_obj_get_style_border_color(mLvglSelf, aStyle))
      .Opacity(lv_obj_get_style_border_opa(mLvglSelf, aStyle))
      .Sides(lv_obj_get_style_border_side(mLvglSelf, aStyle))
      .Width(lv_obj_get_style_border_width(mLvglSelf, aStyle));
}

void UIElement::SetOutline(Outline aNewOutline, lv_style_selector_t aStyle) {
  LvglResourceManger::GetInstance().AttemptNow([this, aNewOutline, aStyle] {
    lv_obj_set_style_outline_color(mLvglSelf, aNewOutline.color, aStyle);
    lv_obj_set_style_outline_opa(mLvglSelf, aNewOutline.opacity, aStyle);
    lv_obj_set_style_outline_width(mLvglSelf, aNewOutline.width, aStyle);
    lv_obj_set_style_outline_pad(mLvglSelf, aNewOutline.padding, aStyle);
  });
};

Outline UIElement::GetOutline(lv_style_selector_t aStyle) {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  return Outline()
      .Color(lv_obj_get_style_outline_color(mLvglSelf, aStyle))
      .Opacity(lv_obj_get_style_outline_opa(mLvglSelf, aStyle))
      .Padding(lv_obj_get_style_outline_pad(mLvglSelf, aStyle))
      .Width(lv_obj_get_style_outline_width(mLvglSelf, aStyle));
};

void UIElement::SetPadding(Padding aNewPadding, lv_style_selector_t aStyle) {
  LvglResourceManger::GetInstance().AttemptNow([this, aNewPadding, aStyle] {
    lv_obj_set_style_pad_top(mLvglSelf, aNewPadding.top, aStyle);
    lv_obj_set_style_pad_bottom(mLvglSelf, aNewPadding.bottom, aStyle);
    lv_obj_set_style_pad_left(mLvglSelf, aNewPadding.left, aStyle);
    lv_obj_set_style_pad_right(mLvglSelf, aNewPadding.right, aStyle);
    lv_obj_set_style_pad_row(mLvglSelf, aNewPadding.row, aStyle);
    lv_obj_set_style_pad_column(mLvglSelf, aNewPadding.column, aStyle);
  });
};

void UIElement::SetAllPadding(lv_coord_t aNewPadding,
                              lv_style_selector_t aStyle) {
  LvglResourceManger::GetInstance().AttemptNow([this, aNewPadding, aStyle] {
    lv_obj_set_style_pad_all(mLvglSelf, aNewPadding, aStyle);
  });
}

Padding UIElement::GetPadding(lv_style_selector_t aStyle) {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  return Padding()
      .Top(lv_obj_get_style_pad_top(mLvglSelf, aStyle))
      .Bottom(lv_obj_get_style_pad_bottom(mLvglSelf, aStyle))
      .Left(lv_obj_get_style_pad_left(mLvglSelf, aStyle))
      .Right(lv_obj_get_style_pad_right(mLvglSelf, aStyle))
      .Row(lv_obj_get_style_pad_row(mLvglSelf, aStyle))
      .Column(lv_obj_get_style_pad_column(mLvglSelf, aStyle));
};

void UIElement::AlignTo(UIElement *anElementToAlignTo, lv_align_t anAlignment,
                        lv_coord_t aXoffset, lv_coord_t aYOffset) {
  LvglResourceManger::GetInstance().AttemptNow([=] {
    lv_obj_align_to(mLvglSelf, anElementToAlignTo->mLvglSelf, anAlignment,
                    aXoffset, aYOffset);
  });
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

//////////////////// Statics //////////////////////////

void UIElement::LvglEventHandler(lv_event_t *anEvent) {
  auto lock = LvglResourceManger::GetInstance().scopeLock();
  reinterpret_cast<UIElement *>(anEvent->user_data)->OnLvglEvent(anEvent);
}

} // namespace UI
