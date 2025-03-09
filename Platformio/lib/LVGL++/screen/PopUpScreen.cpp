#include "PopUpScreen.hpp"
#include "Colors.hpp"
#include "ScreenManager.hpp"

using namespace UI;
using namespace UI::Screen;

PopUpScreen::PopUpScreen(Page::Base::Ptr aPage)
    : Screen::Base(UI::ID::Screens::PopUp) {

  mContentPage = AddElement(std::move(aPage));

  mExitButton = AddNewElement<Widget::Button>(
      [this] { UI::Screen::Manager::getInstance().popScreen(this); });

  mTitle = AddNewElement<Widget::Label>(mContentPage->GetTitle());

  mExitButton->SetWidth(lv_pct(10));
  mExitButton->SetHeight(mExitButton->GetWidth());
  mExitButton->SetBgColor(Color::RED);
  mExitButton->AlignTo(this, LV_ALIGN_TOP_RIGHT, -5, 5);

  mTitle->SetWidth(mExitButton->GetX());
  mTitle->SetHeight(mExitButton->GetHeight());
  mTitle->AlignTo(mExitButton, LV_ALIGN_OUT_LEFT_BOTTOM);
  mTitle->SetTextStyle(mTitle->GetTextStyle().Align(LV_TEXT_ALIGN_CENTER));

  mContentPage->SetHeight(GetHeight() - mExitButton->GetBottom() - 5);
  mContentPage->SetY(mExitButton->GetBottom() + 5);
}

bool PopUpScreen::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return mContentPage->OnKeyEvent(aKeyEvent);
}
