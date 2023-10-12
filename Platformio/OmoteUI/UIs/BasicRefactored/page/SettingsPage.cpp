#include "SettingsPage.hpp"
#include "BackgroundScreen.hpp"
#include "Button.hpp"
#include "Slider.hpp"
#include "List.hpp"
#include "Colors.hpp"
#include "DisplaySettings.hpp"
#include "PopUpScreen.hpp"
#include "ScreenManager.hpp"

using namespace UI::Page;
using namespace UI::Color;

SettingsPage::SettingsPage(std::shared_ptr<HardwareAbstract> aHardware)
    : Base(ID::Pages::Settings),
    mSettingsList(AddElement<Widget::List>(std::make_unique<Widget::List>())),
    mHardware(aHardware) {

  mSettingsList->AddItem("Display",LV_SYMBOL_EYE_OPEN,[this] { PushDisplaySettings(); });
  mSettingsList->AddItem("Wifi",LV_SYMBOL_WIFI,[]{});
}

void SettingsPage::PushDisplaySettings() {
  UI::Screen::Manager::getInstance().pushPopUp(
      std::make_unique<DisplaySettings>(mHardware->display()));
}

