#pragma once
#include "WidgetBase.hpp"

namespace UI::Widget {
class Button : public Base {
public:
  Button(std::function<void()> aOnPressHandler);

  void OnLvglEvent(lv_event_t *anEvent) override;

private:
  std::function<void()> mOnPress;
};

} // namespace UI::Widget