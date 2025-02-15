#pragma once
#include <functional>

#include "Label.hpp"
#include "WidgetBase.hpp"

namespace UI::Widget {
class Button : public Base {
 public:
  Button(std::function<void()> aOnPressHandler,
         std::function<void()> aOnReleaseHandler = nullptr);
  virtual ~Button() = default;

  void SetText(std::string aText);

  // Override in order to pass styling to label
  void SetTextStyle(TextStyle aNewStyle,
                    lv_style_selector_t aStyle = LV_PART_MAIN) override;

 protected:
  void OnLvglEvent(lv_event_t *anEvent) override;

 private:
  Label *mText = nullptr;
  std::function<void()> mOnPress;
  std::function<void()> mOnRelease;
};

}  // namespace UI::Widget