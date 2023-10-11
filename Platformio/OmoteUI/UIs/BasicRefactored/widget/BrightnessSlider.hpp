#pragma once
#include "WidgetBase.hpp"
#include "DisplayAbstract.h"

namespace UI::Widget{
class Slider;
class Label;

class BrightnessSlider : public Base{
public:
    BrightnessSlider(std::shared_ptr<DisplayAbstract> aDisplay);

    void OnShow() override;
    void SetHeight(lv_coord_t aHeight) override;
private:
    std::shared_ptr<DisplayAbstract> mDisplay;
    Slider* mSlider;
    Label* mLabel;
};

}