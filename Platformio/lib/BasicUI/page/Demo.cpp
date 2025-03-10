#include "Demo.hpp"
#include "Slider.hpp"

using namespace UI::Page;

Demo::Demo() : Base(ID::Pages::Demo) {}

void Demo::AddSlider() {
  auto fakeSlider = std::make_unique<Widget::Slider>([](auto data) {});
  fakeSlider->SetHeight(lv_pct(10));
  fakeSlider->SetWidth(GetContentWidth());
  if (sliders.empty()) {
    fakeSlider->AlignTo(this, LV_ALIGN_TOP_MID);
  } else {
    auto nextY = sliders.back()->GetY() + sliders.back()->GetHeight();
    fakeSlider->SetY(nextY + 10);
  }

  sliders.push_back(AddElement(std::move(fakeSlider)));
}

bool Demo::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
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
        RemoveElement(sliders[0]);
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