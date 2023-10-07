#include "SettingsPage.hpp"
#include "BackgroundScreen.hpp"
#include "Button.hpp"

using namespace UI::Page;

SettingsPage::SettingsPage(std::shared_ptr<HardwareAbstract> aHardware)
    : Base(ID::Pages::Settings), mHardware(aHardware) {
  SetBgColor(lv_color_make(255, 0, 0));
  auto button = std::make_unique<UI::Widget::Button>([this] { AddSlider(); });
  button->SetY(0);
  button->SetHeight(lv_pct(10));
  button->SetWidth(lv_pct(10));

  mButton = AddWidget(std::move(button));
}

void SettingsPage::OnShow() {}

void SettingsPage::AddSlider() {
  auto fakeSlider = std::make_unique<UI::Widget::Base>(
      lv_slider_create(UI::Screen::BackgroundScreen::getLvInstance()));
  fakeSlider->SetHeight(lv_pct(10));
  fakeSlider->SetWidth(GetWidth());
  if (sliders.empty()) {
    fakeSlider->SetY(mButton->GetBottom());
  } else {
    auto nextY = sliders.back()->GetY() + sliders.back()->GetHeight();
    fakeSlider->SetY(nextY + 10);
  }

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
    used = Page::Base::OnKeyEvent(aKeyEvent);
    break;
  }
  return used;
}