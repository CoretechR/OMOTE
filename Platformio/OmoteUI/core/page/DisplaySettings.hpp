#pragma once
#include "DisplayAbstract.h"
#include "PageBase.hpp"

namespace UI::Widget {
class Slider;
}

namespace UI::Page {
class DisplaySettings : public Base {
public:
  DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay);

  void OnShow() override;
  void OnHide() override;

  std::string GetTitle() override { return "Display Settings"; };

private:
  std::shared_ptr<DisplayAbstract> mDisplay;
  Widget::Slider *mBrightnessSlider;
};
} // namespace UI::Page
