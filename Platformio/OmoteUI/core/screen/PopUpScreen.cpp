#include "PopUpScreen.hpp"
#include "Colors.hpp"
#include "ScreenManager.hpp"

using namespace UI;
using namespace UI::Screen;

PopUpScreen::PopUpScreen(Page::Base::Ptr aPage)
    : Screen::Base(UI::ID::Screens::PopUp), mContentPage(std::move(aPage)),
      mExitButton(std::make_unique<Widget::Button>(
          [this] { UI::Screen::Manager::getInstance().popScreen(this); })),
      mTitle(std::make_unique<Widget::Label>(mContentPage->GetTitle())) {

  mExitButton->SetWidth(lv_pct(10));
  mExitButton->SetHeight(mExitButton->GetWidth());
  mExitButton->SetBgColor(Color::RED);
  mExitButton->AlignTo(this, LV_ALIGN_TOP_RIGHT, -5, 5);

  mTitle->SetWidth(mExitButton->GetX());
  mTitle->SetHeight(mExitButton->GetHeight());
  mTitle->AlignTo(mExitButton.get(), LV_ALIGN_OUT_LEFT_BOTTOM);
  mTitle->SetTextStyle(mTitle->GetTextStyle().Align(LV_TEXT_ALIGN_CENTER));

  mContentPage->SetHeight(GetHeight() - mExitButton->GetBottom() - 5);
  mContentPage->SetY(mExitButton->GetBottom() + 5);

  AddElement(mContentPage.get());
  AddElement(mExitButton.get());
  AddElement(mTitle.get());
}

bool PopUpScreen::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return mContentPage->OnKeyEvent(aKeyEvent);
}

void PopUpScreen::OnShow() {
  mContentPage->OnShow();
  mExitButton->OnShow();
  mTitle->OnShow();
};
void PopUpScreen::OnHide() {
  mContentPage->OnHide();
  mExitButton->OnHide();
  mTitle->OnHide();
};