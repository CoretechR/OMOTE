#include "widget/HomeAssistant/Light.hpp"
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
}

void Light::SetHeight(lv_coord_t aHeight) {
  Base::SetHeight(aHeight);
  mLabel->SetHeight(aHeight * 0.3);
  mBrightnessSlider->SetHeight(aHeight * 0.7);

  mLabel->AlignTo(this, LV_ALIGN_TOP_MID);
  mBrightnessSlider->AlignTo(mLabel, LV_ALIGN_OUT_BOTTOM_MID);
};
void Light::SetWidth(lv_coord_t aWidth) {
  Base::SetWidth(aWidth);
  mLabel->SetWidth(aWidth);
  mBrightnessSlider->SetWidth(aWidth);
};

} // namespace UI::Widget
