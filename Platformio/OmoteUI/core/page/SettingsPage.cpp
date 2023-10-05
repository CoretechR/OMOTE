#include "SettingsPage.hpp"
#include "BackgroundScreen.hpp"

using namespace UI::Page;

SettingsPage::SettingsPage(std::shared_ptr<HardwareAbstract> aHardware)
    : Base(ID::Pages::Settings), mHardware(aHardware) {
  SetBgColor(lv_color_make(255, 0, 0));
}

void SettingsPage::OnShow() {}

void SettingsPage::AddSlider() {
  auto fakeSlider = std::make_unique<UI::Widget::Base>(
      lv_slider_create(UI::Screen::BackgroundScreen::getLvInstance()));
  fakeSlider->SetHeight(lv_pct(10));

  fakeSlider->SetWidth(GetWidth());
  auto sliderHeight = fakeSlider->GetHeight();
  fakeSlider->SetY(sliderHeight * GetNumWidgets());

  sliders.push_back(AddWidget(std::move(fakeSlider)));
}

bool SettingsPage::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  using id = KeyPressAbstract::KeyId;
  using eventType = KeyPressAbstract::KeyEvent::Type;
  bool used = true;
  switch (aKeyEvent.mId) {
  case id::Aux1:
    if (aKeyEvent.mType == eventType::Press) {
      AddSlider();
    }
    break;
  case id::Aux2:
    if (aKeyEvent.mType == eventType::Release) {
      if (sliders.size() > 0) {
        auto widget = RemoveWidget(sliders[0]);
        sliders.erase(
            sliders.begin()); // sliders is non owning so after removing delete
                              // it from non owning array
      }
    }
    break;
  case id::Aux3:
    break;
  case id::Aux4:
    break;
  default:
    used = false;
    break;
  }
  return used;
}