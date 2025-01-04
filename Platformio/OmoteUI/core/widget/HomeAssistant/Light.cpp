#include "widget/HomeAssistant/Light.hpp"
#include "Colors.hpp"
#include "HomeAssist/HomeAssistAccess.hpp"
#include "Label.hpp"
#include "Slider.hpp"

namespace UI::Widget {
Light::Light(std::string aEntityId)
    : Base(UI::ID::Widgets::INVALID_WIDGET_ID),
      mBrightnessSlider(AddNewElement<Slider>(
          [this](auto aSliderValue) {
            mLightController.SetBrightness(aSliderValue);
          },
          0, 255)),
      mLabel(AddNewElement<Label>(aEntityId)),
      mLightController(HomeAssist::HomeAssistantAccess::GetApiAccess(),
                       aEntityId) {
  mBrightnessSlider->UpdateOnReleaseOnly(true);

  UpdateSlider();
  SetupStyle();
}

Light::~Light() {
  for (auto *style : {&mIndicatorStyle, &mMainStyle}) {
    lv_style_reset(style);
  }
}

void Light::SetupStyle() {
  for (auto *style : {&mIndicatorStyle, &mMainStyle}) {
    lv_style_init(style);
  }
  lv_style_set_bg_color(&mIndicatorStyle, Color::NETFLIX_RED);
  lv_style_set_pad_all(&mMainStyle, 10);
  mBrightnessSlider->AddStyle(&mMainStyle, LV_PART_KNOB);
  mBrightnessSlider->AddStyle(&mIndicatorStyle,
                              LV_PART_INDICATOR | LV_STATE_PRESSED);
}

void Light::SetHeight(lv_coord_t aHeight) {
  Base::SetHeight(aHeight);

  auto contentHeight = GetContentHeight();
  auto labelHeight = contentHeight * 0.3;
  mLabel->SetHeight(labelHeight);

  auto sliderHeight = contentHeight * 0.2;
  mBrightnessSlider->SetHeight(sliderHeight);
  auto xRoomForSlider = contentHeight - labelHeight;
  auto sliderXOffset = (xRoomForSlider / 2) - (sliderHeight / 2);

  mLabel->AlignTo(this, LV_ALIGN_TOP_MID);
  mBrightnessSlider->AlignTo(mLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, sliderXOffset);
};
void Light::SetWidth(lv_coord_t aWidth) {
  Base::SetWidth(aWidth);
  auto contentWidth = GetContentWidth();
  mLabel->SetWidth(contentWidth);
  auto sliderWidth = contentWidth * 0.8;
  mBrightnessSlider->SetWidth(sliderWidth);

  mBrightnessSlider->SetX((contentWidth - sliderWidth) / 2);
};

void Light::UpdateSlider() {
  mLightController.Refresh();
  auto currentBrightness = mLightController.getBrightness();
  mBrightnessSlider->SetValue(currentBrightness);
}

void Light::OnShow() { UpdateSlider(); }

} // namespace UI::Widget
