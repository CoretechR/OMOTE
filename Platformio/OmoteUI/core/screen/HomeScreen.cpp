#include "HomeScreen.hpp"
#include "Colors.hpp"
#include "SettingsPage.hpp"

using namespace UI::Screen;

HomeScreen::HomeScreen(std::shared_ptr<HardwareAbstract> aHardware)
    : Base(UI::ID::Screens::Home), mHardware(aHardware) {
  SetBgColor(UI::Color::BLACK);
  SetPushAnimation(LV_SCR_LOAD_ANIM_FADE_IN);

  mTabView = AddElement<Page::TabView>(std::make_unique<Page::TabView>(
      ID(ID::Pages::INVALID_PAGE_ID))); // Adds Tabview to Homescreen

  // Adds pages to the Tab view
  mTabView->AddTab(std::make_unique<Page::SettingsPage>(aHardware), "Settings");
  mTabView->AddTab(std::make_unique<Page::SettingsPage>(aHardware),
                   "Settings1");
}

void HomeScreen::SetBgColor(lv_color_t value, lv_style_selector_t selector) {
  mTabView->SetBgColor(value, selector);
  UI::UIElement::SetBgColor(value, selector);
}

bool HomeScreen::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return false;
};
