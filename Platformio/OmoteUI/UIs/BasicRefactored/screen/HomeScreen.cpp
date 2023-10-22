#include "HomeScreen.hpp"
#include "Colors.hpp"
#include "Demo.hpp"
#include "SettingsPage.hpp"

using namespace UI::Screen;

HomeScreen::HomeScreen()
    : Base(UI::ID::Screens::Home),
      mTabView(AddElement<Page::TabView>(
          std::make_unique<Page::TabView>(ID(ID::Pages::INVALID_PAGE_ID)))) {

  SetBgColor(UI::Color::BLACK);
  SetPushAnimation(LV_SCR_LOAD_ANIM_FADE_IN);

  // Adds pages to the Tab view
  mTabView->AddTab(std::make_unique<Page::SettingsPage>());
  mTabView->AddTab(std::make_unique<Page::Demo>());
}

void HomeScreen::SetBgColor(lv_color_t value, lv_style_selector_t selector) {
  mTabView->SetBgColor(value, selector);
  UI::UIElement::SetBgColor(value, selector);
}

bool HomeScreen::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return false;
};
