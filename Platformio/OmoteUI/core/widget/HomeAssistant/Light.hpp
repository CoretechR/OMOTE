#pragma once
#include "HomeAssist/Light.hpp"

#include "WidgetBase.hpp"

namespace UI::Widget {
class Label;
class Slider;

class Light : public Base {
public:
  Light(std::string aEntityId);
  ~Light() override = default;

  void SetHeight(lv_coord_t aHeight) override;
  void SetWidth(lv_coord_t aWidth) override;

private:
  Label *mLabel;
  Slider *mBrightnessSlider;
  HomeAssist::Light mLightController;
};

} // namespace UI::Widget