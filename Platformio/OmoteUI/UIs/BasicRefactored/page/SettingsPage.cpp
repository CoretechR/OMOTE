#include "SettingsPage.hpp"
#include "BackgroundScreen.hpp"
#include "Button.hpp"
#include "Colors.hpp"
#include "DisplaySettings.hpp"
#include "HardwareFactory.hpp"
#include "List.hpp"
#include "PopUpScreen.hpp"
#include "ScreenManager.hpp"
#include "Slider.hpp"
#include "SystemSettings.hpp"
#include "WifiSettings.hpp"

using namespace UI::Page;
using namespace UI::Color;

SettingsPage::SettingsPage()
    : Base(ID::Pages::Settings), mSettingsList(AddElement<Widget::List>(
                                     std::make_unique<Widget::List>())) {

  mSettingsList->AddItem("Display", LV_SYMBOL_EYE_OPEN,
                         [this] { PushDisplaySettings(); });
  mSettingsList->AddItem("Wifi", LV_SYMBOL_WIFI,
                         [this] { PushWifiSettings(); });
  mSettingsList->AddItem("System", LV_SYMBOL_SETTINGS,
                         [this] { PushSystemSettings(); });
}

void SettingsPage::PushDisplaySettings() {
  UI::Screen::Manager::getInstance().pushPopUp(
      std::make_unique<DisplaySettings>(
          HardwareFactory::getAbstract().display()));
}

void SettingsPage::PushSystemSettings() {
  UI::Screen::Manager::getInstance().pushPopUp(
      std::make_unique<SystemSettings>());
}

void SettingsPage::PushWifiSettings() {
  UI::Screen::Manager::getInstance().pushPopUp(
      std::make_unique<WifiSettings>(HardwareFactory::getAbstract().wifi()));
}
