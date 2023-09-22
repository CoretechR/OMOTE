#include "HomeScreen.hpp"

using namespace UI::Screen;

HomeScreen::HomeScreen()
    : Base(UI::ID::Screens::Home), mMyTabView(ID(ID::Pages::INVALID_PAGE_ID)),
      mTabView(nullptr) {
  AddElement(&mMyTabView);
  mMyTabView.SetBgColor(lv_color_black());
  SetBgColor(lv_color_white());
  SetPushAnimation(LV_SCR_LOAD_ANIM_FADE_IN);
}

// void HomeScreen::SetBgColor(lv_color_t value, lv_style_selector_t selector) {
//   mTabView.SetBgColor(value, selector);
//   UI::UIElement::SetBgColor(value, selector);
// }

void HomeScreen::AddPage(Page::Base::Ptr aPage) {
  // mTabView.AddElement(aPage.get());
  mPages.push_back(std::move(aPage));
}