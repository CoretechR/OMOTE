#include "BrightnessSlider.hpp"
#include "Label.hpp"
#include "Slider.hpp"

using namespace UI::Widget;

BrightnessSlider::BrightnessSlider(std::shared_ptr<DisplayAbstract> aDisplay)
    : Base(ID::Widgets::BrightnessSlider), mDisplay(aDisplay),
      mSlider(AddElement<Widget::Slider>(std::make_unique<Slider>(
          [this](auto aNewBrightness) {
            mDisplay->setBrightness(aNewBrightness);
          },
          0, 255))),
      mLabel(AddElement<Widget::Label>(std::make_unique<Label>("Brightness"))) {
  mLabel->AlignTo(this, LV_ALIGN_TOP_MID);
  mSlider->AlignTo(mLabel, LV_ALIGN_OUT_BOTTOM_MID);
  mSlider->SetWidth(GetContentWidth() - GetContentWidth() * 0.25f);
  SetVisiblity(false);
}

void BrightnessSlider::OnShow() {
  mSlider->SetValue(mDisplay->getBrightness());
  Base::OnShow();
}

void BrightnessSlider::SetHeight(lv_coord_t aHeight) {
  Base::SetHeight(aHeight);
  auto labelHeight = 12;
  auto sliderHeight = aHeight * 0.60f - labelHeight;
  mLabel->SetHeight(labelHeight);
  mSlider->SetHeight(sliderHeight);
  mLabel->AlignTo(this, LV_ALIGN_TOP_MID);
  mSlider->AlignTo(mLabel, LV_ALIGN_OUT_BOTTOM_MID, 0,
                   mLabel->GetContentHeight());
}

bool BrightnessSlider::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  using ID = KeyPressAbstract::KeyId;
  using Type = KeyPressAbstract::KeyEvent::Type;
  if (aKeyEvent.mId == ID::Aux1 && aKeyEvent.mType == Type::Press) {
    SetVisiblity(!IsSetVisible());
    return true;
  }
  return false;
}