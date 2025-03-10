#pragma once
#include "WidgetBase.hpp"

#include <string>

namespace UI::Widget {
class Label : public Base {
public:
  Label(std::string aText);

  void SetText(std::string aText);

  void SetLongMode(lv_label_long_mode_t aLongMode);
};

} // namespace UI::Widget
