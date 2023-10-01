#include "HomeScreen.hpp"
#include "SettingsPage.hpp"

using namespace UI::Screen;

HomeScreen::HomeScreen()
    : Base(UI::ID::Screens::Home), mTabView(ID(ID::Pages::INVALID_PAGE_ID)) {
  SetBgColor(lv_color_black());
  SetPushAnimation(LV_SCR_LOAD_ANIM_FADE_IN);

  AddElement(&mTabView); // Adds Tabview to Homescreen

  // Adds pages to the Tab view
  mTabView.AddTab(std::make_unique<Page::SettingsPage>(), "Settings");
  mTabView.AddTab(std::make_unique<Page::SettingsPage>(), "Settings1");
}

void HomeScreen::SetBgColor(lv_color_t value, lv_style_selector_t selector) {
  mTabView.SetBgColor(value, selector);
  UI::UIElement::SetBgColor(value, selector);
}

bool HomeScreen::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return false;
};

bool HomeScreen::KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  if (OnKeyEvent(aKeyEvent)) {
    return true;
  }
  return mTabView.KeyEvent(aKeyEvent);
};
