#include "SystemSettings.hpp"
#include "HardwareFactory.hpp"
#include "Label.hpp"

using namespace UI::Page;

SystemSettings::SystemSettings()
    : Base(ID::Pages::SystemSettings),
      mTimeoutLabel(AddElement<Widget::Label>(
          std::make_unique<Widget::Label>("TimeOut"))),
      mScreenTimeOutDropDown(AddElement<Widget::DropDown<int>>(
          std::make_unique<Widget::DropDown<int>>([this](int aTimeout) {
            HardwareFactory::getAbstract().setSleepTimeout(aTimeout);
          }))) {

  mTimeoutLabel->AlignTo(this, LV_ALIGN_TOP_MID);
  mTimeoutLabel->SetHeight(15);

  mScreenTimeOutDropDown->SetHeight(30);
  mScreenTimeOutDropDown->SetWidth(GetContentWidth());
  mScreenTimeOutDropDown->AddItem("10 Seconds", 10000);
  mScreenTimeOutDropDown->AddItem("15 Seconds", 15000);
  mScreenTimeOutDropDown->AddItem("20 Seconds", 20000);
  mScreenTimeOutDropDown->AlignTo(mTimeoutLabel, LV_ALIGN_OUT_BOTTOM_MID);
  mScreenTimeOutDropDown->SetSelected(
      HardwareFactory::getAbstract().getSleepTimeout());
}