#include "DisplaySettings.hpp"
#include "BrightnessSlider.hpp"
#include "Label.hpp"

using namespace UI::Page;

DisplaySettings::DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay)
    : Base(UI::ID::Pages::DisplaySettings), mDisplay(aDisplay),
      mBrightnessSlider(AddElement<Widget::BrightnessSlider>(
          std::make_unique<Widget::BrightnessSlider>(mDisplay))) {
  SetBgColor(Color::GREY);

  mBrightnessSlider->SetWidth(GetContentWidth());
  mBrightnessSlider->SetHeight(80);
  mBrightnessSlider->AlignTo(this, LV_ALIGN_TOP_MID);
}
