#pragma once
#include "Label.hpp"
#include "WidgetBase.hpp"
namespace UI::Widget {
class Button : public Base {
public:
  Button(std::function<void()> aOnPressHandler);
  virtual ~Button() = default;

  void SetText(std::string aText);

protected:
  void OnLvglEvent(lv_event_t *anEvent) override;

private:
  Label *mText = nullptr;
  std::function<void()> mOnPress;
};

} // namespace UI::Widget