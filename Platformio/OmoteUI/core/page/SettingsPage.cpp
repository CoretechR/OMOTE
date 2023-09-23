#include "BackgroundScreen.hpp"
#include "SettingsPage.hpp"

using namespace UI::Page;

SettingsPage::SettingsPage() : Base(ID::Pages::Settings) {
  SetBgColor(lv_color_make(255, 0, 0));
}

void SettingsPage::OnShow() {
  auto fakeSlider = std::make_unique<UI::Widget::Base>(
      lv_slider_create(UI::Screen::BackgroundScreen::getLvInstance()));
  fakeSlider->SetHeight(lv_pct(10));

  fakeSlider->SetWidth(GetWidth());
  auto sliderHeight = fakeSlider->GetHeight();
  fakeSlider->SetY(sliderHeight * GetNumWidgets());

  AddWidget(std::move(fakeSlider));
}