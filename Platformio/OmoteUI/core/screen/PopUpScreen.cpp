#include "PopUpScreen.hpp"
#include "Colors.hpp"
#include "ScreenManager.hpp"

using namespace UI;
using namespace UI::Screen;

PopUpScreen::PopUpScreen(Page::Base::Ptr aPage)
    : Screen::Base(UI::ID::Screens::PopUp), mContentPage(std::move(aPage)),
      mExitButton(std::make_unique<Widget::Button>(
          [this] { UI::Screen::Manager::getInstance().popScreen(this); })) {

  mExitButton->SetWidth(30);
  mExitButton->SetHeight(30);
  mExitButton->SetBgColor(Color::RED);
  mExitButton->AlignTo(this, LV_ALIGN_TOP_RIGHT, -5, 5);

  AddElement(mContentPage.get());
  AddElement(mExitButton.get());
}

bool PopUpScreen::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return mContentPage->OnKeyEvent(aKeyEvent);
}