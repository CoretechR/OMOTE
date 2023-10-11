#include "DisplaySettings.hpp"
#include "Slider.hpp"

using namespace UI::Page;

DisplaySettings::DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay)
    : Base(UI::ID::Pages::DisplaySettings), mDisplay(aDisplay),
      mBrightnessSlider(
          AddElement<Widget::Slider>(std::make_unique<Widget::Slider>(
              [this](auto aNewBrightness) {
                mDisplay->setBrightness(aNewBrightness);
              },
              0, 255))) {
  SetBgColor(Color::GREY);
  auto usableWidth = GetContentWidth();

  mBrightnessSlider->SetWidth(usableWidth - (usableWidth * 0.20f));
  mBrightnessSlider->SetHeight(lv_pct(10));
  mBrightnessSlider->AlignTo(this, LV_ALIGN_TOP_MID);
}

void DisplaySettings::OnShow() {
  mBrightnessSlider->SetValue(mDisplay->getBrightness());
};

void DisplaySettings::OnHide(){

};