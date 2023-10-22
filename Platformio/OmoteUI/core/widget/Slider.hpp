#include "WidgetBase.hpp"

namespace UI::Widget {
class Slider : public Base {
public:
  Slider(std::function<void(int32_t)> OnSliderValueChange, int32_t aMinVal = 0,
         int32_t aMaxVal = 100);

  int32_t GetValue();
  void SetValue(int32_t aValue, lv_anim_enable_t aIsAnimate = LV_ANIM_ON);

protected:
  void OnLvglEvent(lv_event_t *anEvent) override;

private:
  std::function<void(int32_t)> mOnSliderChange;
};

} // namespace UI::Widget
