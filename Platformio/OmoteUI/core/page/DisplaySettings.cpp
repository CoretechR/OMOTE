#include "DisplaySettings.hpp"
#include "Slider.hpp"

using namespace UI::Page;

DisplaySettings::DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay)
    : Base(UI::ID::Pages::DisplaySettings), mDisplay(aDisplay) {
  auto slider = std::make_unique<Widget::Slider>(
      [this](auto aNewBrightness) { mDisplay->setBrightness(aNewBrightness); },
      0, 255);
  slider->AlignTo(this, LV_ALIGN_CENTER);
  slider->SetWidth(GetContentWidth());
  slider->SetHeight(lv_pct(10));
  AddWidget(std::move(slider));
}