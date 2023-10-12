#include "Slider.hpp"
#include "BackgroundScreen.hpp"
#include "LvglResourceManager.hpp"

using namespace UI::Widget;

Slider::Slider(std::function<void(int32_t)> aOnSliderValueChange,
               int32_t aMinVal, int32_t aMaxVal)
    : Base(lv_slider_create(UI::Screen::BackgroundScreen::getLvInstance()),
           ID::Widgets::Slider),
      mOnSliderChange(std::move(aOnSliderValueChange)) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_slider_set_range(LvglSelf(), aMinVal, aMaxVal);
}

int32_t Slider::GetValue() {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  return lv_slider_get_value(LvglSelf());
}

void Slider::SetValue(int32_t aValue, lv_anim_enable_t aIsAnimate) {
  LvglResourceManager::GetInstance().AttemptNow([this, aValue, aIsAnimate] {
    lv_slider_set_value(LvglSelf(), aValue, aIsAnimate);
  });
}

void Slider::OnLvglEvent(lv_event_t *anEvent) {
  if (anEvent->code == LV_EVENT_VALUE_CHANGED) {
    mOnSliderChange(GetValue());
  }
}