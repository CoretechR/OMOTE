#include "Button.hpp"
#include "BackgroundScreen.hpp"

using namespace UI::Widget;

Button::Button(std::function<void()> aOnPressHandler,
               std::function<void()> aOnReleaseHandler)
    : Base(lv_btn_create(UI::Screen::BackgroundScreen::getLvInstance()),
           ID::Widgets::Button),
      mOnPress(aOnPressHandler),
      mOnRelease(aOnReleaseHandler) {}

void Button::OnLvglEvent(lv_event_t *anEvent) {
  if (lv_event_get_code(anEvent) == LV_EVENT_PRESSED && mOnPress) {
    mOnPress();
  } else if (lv_event_get_code(anEvent) == LV_EVENT_RELEASED && mOnRelease) {
    mOnRelease();
  }
};

void Button::SetTextStyle(TextStyle aNewStyle, lv_style_selector_t aStyle) {
  if (mText) {
    mText->SetTextStyle(aNewStyle, aStyle);
  }
  UIElement::SetTextStyle(aNewStyle, aStyle);
};

void Button::SetText(std::string aText) {
  if (!mText) {
    mText = AddNewElement<Label>(aText);
    mText->SetTextStyle(UI::TextStyle().Align(LV_TEXT_ALIGN_CENTER));
  }
  mText->SetText(aText);
}
