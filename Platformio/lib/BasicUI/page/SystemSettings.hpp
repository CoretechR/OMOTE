#pragma once
#include "DropDown.hpp"
#include "PageBase.hpp"

namespace UI::Widget {
class Label;
}

namespace UI::Page {

class SystemSettings : public Base {
public:
  SystemSettings();

protected:
  std::string GetTitle() override { return "System Settings"; }

private:
  Widget::Label *mTimeoutLabel;
  Widget::DropDown<int> *mScreenTimeOutDropDown;
};

} // namespace UI::Page