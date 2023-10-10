#pragma once
#include "WidgetBase.hpp"

#include <string>

namespace UI::Widget {
class Label : public Base {
public:
  Label(std::string aText);
};

} // namespace UI::Widget
