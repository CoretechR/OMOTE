#pragma once
#include "HomeAssistant/Api/Rest/Light.hpp"
#include "WidgetBase.hpp"

namespace UI::Widget {
class Label;
class Slider;

class Light : public Base {
 public:
  Light(std::string aEntityId);
  ~Light() override;

  void SetHeight(lv_coord_t aHeight) override;
  void SetWidth(lv_coord_t aWidth) override;

  void OnShow() override;

  virtual void SetupStyle();
  lv_style_t mIndicatorStyle;
  lv_style_t mMainStyle;

 private:
  void UpdateSlider();

  Label *mLabel;
  Slider *mBrightnessSlider;
  HomeAssist::Light mLightController;
};

}  // namespace UI::Widget