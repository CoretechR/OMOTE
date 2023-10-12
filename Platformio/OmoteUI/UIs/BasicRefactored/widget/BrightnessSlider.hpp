#pragma once
#include "DisplayAbstract.h"
#include "WidgetBase.hpp"

namespace UI::Widget {
class Slider;
class Label;

class BrightnessSlider : public Base {
public:
  BrightnessSlider(std::shared_ptr<DisplayAbstract> aDisplay);

  void SetHeight(lv_coord_t aHeight) override;

protected:
  void OnShow() override;
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  std::shared_ptr<DisplayAbstract> mDisplay;
  Slider *mSlider;
  Label *mLabel;
};

} // namespace UI::Widget