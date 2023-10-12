#include "UIElement.hpp"
#include "LvglResourceManager.hpp"

namespace UI {
UIElement::UIElement(lv_obj_t *aLvglSelf, ID aId)
    : mLvglSelf(aLvglSelf), mId(aId) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  mLvglSelf->user_data = this;
  // Register Handler so that all object are able to override OnLvglEvent to
  // handle events easilyOnLvEvent
  lv_obj_add_event_cb(mLvglSelf, UIElement::LvglEventHandler, LV_EVENT_ALL,
                      this);
}

UIElement::~UIElement() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  if (lv_obj_is_valid(LvglSelf())) {
    if (mLvglKeepAliveTime > 0) {
      lv_obj_del_delayed(LvglSelf(), mLvglKeepAliveTime);
    } else {
      lv_obj_del(LvglSelf());
    }
    lv_obj_remove_event_cb(mLvglSelf, UIElement::LvglEventHandler);
  }
}

UIElement *UIElement::AddElement(UIElement::Ptr anUIElement) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_obj_set_parent(anUIElement->mLvglSelf, mLvglSelf);
  mContainedElements.push_back(std::move(anUIElement));
  return mContainedElements[mContainedElements.size() - 1].get();
}

UIElement::Ptr UIElement::RemoveElement(UIElement *anElementRef) {
  auto ElemToRemoveIter =
      std::find_if(mContainedElements.begin(), mContainedElements.end(),
                   [anElementRef](auto &anElement) {
                     return anElement.get() == anElementRef;
                   });
  if (ElemToRemoveIter != mContainedElements.end()) {
    auto widget = std::move(*ElemToRemoveIter);
    mContainedElements.erase(ElemToRemoveIter);
    return widget;
  }
  return nullptr;
}

bool UIElement::KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  if (OnKeyEvent(aKeyEvent)) {
    return true;
  }
  for (auto &elem : mContainedElements) {
    if (elem->KeyEvent(aKeyEvent)) {
      return true;
    }
  }
  return false;
};

bool UIElement::IsVisible() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  return lv_obj_is_visible(mLvglSelf);
}

bool UIElement::IsSetVisible() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  return !lv_obj_has_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN);
}

void UIElement::SetWidth(lv_coord_t aWidth) {
  LvglResourceManager::GetInstance().AttemptNow(
      [this, aWidth] { lv_obj_set_width(mLvglSelf, aWidth); });
}

void UIElement::SetHeight(lv_coord_t aHeight) {
  LvglResourceManager::GetInstance().AttemptNow(
      [this, aHeight] { lv_obj_set_height(mLvglSelf, aHeight); });
}

lv_coord_t UIElement::GetHeight() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_height(mLvglSelf);
};

lv_coord_t UIElement::GetWidth() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_width(mLvglSelf);
}

void UIElement::SetContentWidth(lv_coord_t aWidth) {
  LvglResourceManager::GetInstance().AttemptNow(
      [this, aWidth] { lv_obj_set_content_width(mLvglSelf, aWidth); });
}
void UIElement::SetContentHeight(lv_coord_t aHeight) {
  LvglResourceManager::GetInstance().AttemptNow(
      [this, aHeight] { lv_obj_set_content_height(mLvglSelf, aHeight); });
}

lv_coord_t UIElement::GetContentWidth() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_content_width(mLvglSelf);
}
lv_coord_t UIElement::GetContentHeight() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_content_height(mLvglSelf);
}

void UIElement::SetY(lv_coord_t aY) {
  LvglResourceManager::GetInstance().AttemptNow(
      [this, aY] { lv_obj_set_y(mLvglSelf, aY); });
};
void UIElement::SetX(lv_coord_t aX) {
  LvglResourceManager::GetInstance().AttemptNow(
      [this, aX] { lv_obj_set_x(mLvglSelf, aX); });
}

lv_coord_t UIElement::GetY() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_y(mLvglSelf);
}
lv_coord_t UIElement::GetX() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_obj_update_layout(mLvglSelf);
  return lv_obj_get_x(mLvglSelf);
}

lv_coord_t UIElement::GetBottom() { return GetY() + GetHeight(); };

void UIElement::SetBorder(Border aNewBorder, lv_style_selector_t aStyle) {
  LvglResourceManager::GetInstance().AttemptNow([this, aNewBorder, aStyle] {
    lv_obj_set_style_border_color(mLvglSelf, aNewBorder.color, aStyle);
    lv_obj_set_style_border_opa(mLvglSelf, aNewBorder.opacity, aStyle);
    lv_obj_set_style_border_side(mLvglSelf, aNewBorder.sides, aStyle);
    lv_obj_set_style_border_width(mLvglSelf, aNewBorder.width, aStyle);
  });
}

Border UIElement::GetBorder(lv_style_selector_t aStyle) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  return Border()
      .Color(lv_obj_get_style_border_color(mLvglSelf, aStyle))
      .Opacity(lv_obj_get_style_border_opa(mLvglSelf, aStyle))
      .Sides(lv_obj_get_style_border_side(mLvglSelf, aStyle))
      .Width(lv_obj_get_style_border_width(mLvglSelf, aStyle));
}

void UIElement::SetOutline(Outline aNewOutline, lv_style_selector_t aStyle) {
  LvglResourceManager::GetInstance().AttemptNow([this, aNewOutline, aStyle] {
    lv_obj_set_style_outline_color(mLvglSelf, aNewOutline.color, aStyle);
    lv_obj_set_style_outline_opa(mLvglSelf, aNewOutline.opacity, aStyle);
    lv_obj_set_style_outline_width(mLvglSelf, aNewOutline.width, aStyle);
    lv_obj_set_style_outline_pad(mLvglSelf, aNewOutline.padding, aStyle);
  });
};

Outline UIElement::GetOutline(lv_style_selector_t aStyle) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  return Outline()
      .Color(lv_obj_get_style_outline_color(mLvglSelf, aStyle))
      .Opacity(lv_obj_get_style_outline_opa(mLvglSelf, aStyle))
      .Padding(lv_obj_get_style_outline_pad(mLvglSelf, aStyle))
      .Width(lv_obj_get_style_outline_width(mLvglSelf, aStyle));
};

void UIElement::SetPadding(Padding aNewPadding, lv_style_selector_t aStyle) {
  LvglResourceManager::GetInstance().AttemptNow([this, aNewPadding, aStyle] {
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
  LvglResourceManager::GetInstance().AttemptNow([this, aNewPadding, aStyle] {
    lv_obj_set_style_pad_all(mLvglSelf, aNewPadding, aStyle);
  });
}

Padding UIElement::GetPadding(lv_style_selector_t aStyle) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  return Padding()
      .Top(lv_obj_get_style_pad_top(mLvglSelf, aStyle))
      .Bottom(lv_obj_get_style_pad_bottom(mLvglSelf, aStyle))
      .Left(lv_obj_get_style_pad_left(mLvglSelf, aStyle))
      .Right(lv_obj_get_style_pad_right(mLvglSelf, aStyle))
      .Row(lv_obj_get_style_pad_row(mLvglSelf, aStyle))
      .Column(lv_obj_get_style_pad_column(mLvglSelf, aStyle));
};

void UIElement::SetTextStyle(TextStyle aNewTextStyle,
                             lv_style_selector_t aStyle) {
  LvglResourceManager::GetInstance().AttemptNow([this, aNewTextStyle, aStyle] {
    lv_obj_set_style_text_align(mLvglSelf, aNewTextStyle.align, aStyle);
    lv_obj_set_style_text_color(mLvglSelf, aNewTextStyle.color, aStyle);
    lv_obj_set_style_text_decor(mLvglSelf, aNewTextStyle.decor, aStyle);
    lv_obj_set_style_text_font(mLvglSelf, aNewTextStyle.font, aStyle);
    lv_obj_set_style_text_letter_space(mLvglSelf, aNewTextStyle.letter_space,
                                       aStyle);
    lv_obj_set_style_text_line_space(mLvglSelf, aNewTextStyle.line_space,
                                     aStyle);
    lv_obj_set_style_text_opa(mLvglSelf, aNewTextStyle.opacity, aStyle);
  });
}

TextStyle UIElement::GetTextStyle(lv_style_selector_t aStyle) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  return TextStyle()
      .Align(lv_obj_get_style_text_align(mLvglSelf, aStyle))
      .Color(lv_obj_get_style_text_color(mLvglSelf, aStyle))
      .Decor(lv_obj_get_style_text_decor(mLvglSelf, aStyle))
      .Font(lv_obj_get_style_text_font(mLvglSelf, aStyle))
      .LetterSpacing(lv_obj_get_style_text_letter_space(mLvglSelf, aStyle))
      .LineSpacing(lv_obj_get_style_text_line_space(mLvglSelf, aStyle))
      .Opacity(lv_obj_get_style_text_opa(mLvglSelf, aStyle));
}

void UIElement::AlignTo(UIElement *anElementToAlignTo, lv_align_t anAlignment,
                        lv_coord_t aXoffset, lv_coord_t aYOffset) {
  LvglResourceManager::GetInstance().AttemptNow([=] {
    lv_obj_align_to(mLvglSelf, anElementToAlignTo->mLvglSelf, anAlignment,
                    aXoffset, aYOffset);
  });
}

void UIElement::SetVisiblity(bool aVisible) {
  if (aVisible == !lv_obj_has_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN)) {
    return;
  }
  if (aVisible) {
    Show();
  } else {
    Hide();
  }
}

void UIElement::SetBgColor(lv_color_t aColor, lv_style_selector_t aStyle) {
  LvglResourceManager::GetInstance().AttemptNow([this, aColor, aStyle] {
    lv_obj_set_style_bg_color(mLvglSelf, aColor, aStyle);
  });
};

void UIElement::SetBgOpacity(lv_opa_t aOpacity, lv_style_selector_t aStyle) {
  LvglResourceManager::GetInstance().AttemptNow([this, aOpacity, aStyle] {
    lv_obj_set_style_bg_opa(mLvglSelf, aOpacity, aStyle);
  });
}

void UIElement::StartLvglEventHandler() {
  if (mIsHandlingLvglEvents) {
    return;
  }
  lv_obj_add_event_cb(mLvglSelf, UIElement::LvglEventHandler, LV_EVENT_ALL,
                      this);
  mIsHandlingLvglEvents = true;
}
void UIElement::StopLvglEventHandler() {
  if (!mIsHandlingLvglEvents) {
    return;
  }
  lv_obj_remove_event_cb_with_user_data(mLvglSelf, UIElement::LvglEventHandler,
                                        this);
  mIsHandlingLvglEvents = false;
}

void UIElement::Show() {
  if (IsSetVisible()) {
    return;
  }
  {
    auto lock = LvglResourceManager::GetInstance().scopeLock();
    lv_obj_clear_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN);
  }
  OnShow();
}

void UIElement::Hide() {
  if (!IsSetVisible()) {
    return;
  }
  {
    auto lock = LvglResourceManager::GetInstance().scopeLock();
    lv_obj_add_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN);
  }
  OnHide();
}

void UIElement::OnHide() {
  for (auto &elem : mContainedElements) {
    if (!IsSetVisible()) {
      elem->OnHide();
    }
  }
}

void UIElement::OnShow() {
  for (auto &elem : mContainedElements) {
    if (!lv_obj_has_flag(mLvglSelf, LV_OBJ_FLAG_HIDDEN)) {
      elem->OnShow();
    }
  }
}

//////////////////// Statics //////////////////////////

void UIElement::LvglEventHandler(lv_event_t *anEvent) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  reinterpret_cast<UIElement *>(anEvent->user_data)->OnLvglEvent(anEvent);
}

} // namespace UI
