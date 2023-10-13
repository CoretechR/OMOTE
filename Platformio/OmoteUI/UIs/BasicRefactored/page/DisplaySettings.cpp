#include "DisplaySettings.hpp"
#include "BrightnessSlider.hpp"

using namespace UI::Page;

DisplaySettings::DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay)
    : Base(UI::ID::Pages::DisplaySettings), mDisplay(aDisplay),
      mBrightnessSlider(AddElement<Widget::BrightnessSlider>(
          std::make_unique<Widget::BrightnessSlider>(mDisplay))),
      mScreenTimeOutDropDown(AddElement<Widget::DropDown<int>>(
          std::make_unique<Widget::DropDown<int>>([this](int aTimeout) {

          }))) {
  SetBgColor(Color::GREY);

  mBrightnessSlider->SetWidth(GetContentWidth());
  mBrightnessSlider->SetHeight(80);
  mBrightnessSlider->AlignTo(this, LV_ALIGN_TOP_MID);

  mScreenTimeOutDropDown->SetHeight(30);
  mScreenTimeOutDropDown->SetWidth(GetContentWidth());
  mScreenTimeOutDropDown->AddItem("10 Seconds", 10);
  mScreenTimeOutDropDown->AddItem("15 Seconds", 15);
  mScreenTimeOutDropDown->AddItem("20 Seconds", 20);
  mScreenTimeOutDropDown->AlignTo(mBrightnessSlider, LV_ALIGN_OUT_BOTTOM_MID);
}
