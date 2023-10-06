#include "PopUpScreen.hpp"

using namespace UI;
using namespace UI::Screen;

PopUpScreen::PopUpScreen(Page::Base::Ptr aPage)
    : Screen::Base(UI::ID::Screens::PopUp), mContentPage(std::move(aPage)) {

  AddElement(mContentPage.get());
}

bool PopUpScreen::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return mContentPage->OnKeyEvent(aKeyEvent);
}