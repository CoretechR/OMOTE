#include "BrightnessSlider.hpp"
#include "Label.hpp"
#include "Slider.hpp"

using namespace UI::Widget;


BrightnessSlider::BrightnessSlider(std::shared_ptr<DisplayAbstract> aDisplay): Base(ID::Widgets::BrightnessSlider),
  mDisplay(aDisplay),
  mSlider(AddElement<Widget::Slider>(std::make_unique<Slider>(
    [this](auto aNewBrightness){
        mDisplay->setBrightness(aNewBrightness);
    },0,255))),
  mLabel(AddElement<Widget::Label>(std::make_unique<Label>("Brightness")))
{
    mLabel->AlignTo(this,LV_ALIGN_TOP_MID);
    mSlider->AlignTo(mLabel,LV_ALIGN_OUT_BOTTOM_MID);
    mSlider->SetWidth(GetContentWidth() - 20);
}

void BrightnessSlider::OnShow(){
    mSlider->SetValue(mDisplay->getBrightness());
}

void BrightnessSlider::SetHeight(lv_coord_t aHeight){
    Base::SetHeight(aHeight);
    auto labelHeight = GetContentHeight() * 0.25f;
    auto sliderHeight = aHeight - labelHeight;
    mLabel->SetHeight(labelHeight);
    mSlider->SetHeight(sliderHeight);
    mLabel->AlignTo(this,LV_ALIGN_TOP_MID);
    mSlider->AlignTo(mLabel,LV_ALIGN_OUT_BOTTOM_MID);
}