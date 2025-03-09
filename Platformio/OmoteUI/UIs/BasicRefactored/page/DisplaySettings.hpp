#pragma once
#include "DropDown.hpp"
#include "Hardware/DisplayAbstract.h"
#include "PageBase.hpp"

namespace UI::Widget {
class BrightnessSlider;
}  // namespace UI::Widget

namespace UI::Page {
class DisplaySettings : public Base {
 public:
  DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay);

  std::string GetTitle() override { return "Display Settings"; };

 private:
  std::shared_ptr<DisplayAbstract> mDisplay;
  Widget::BrightnessSlider *mBrightnessSlider;
};
}  // namespace UI::Page
