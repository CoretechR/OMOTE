#pragma once
#include "DropDown.hpp"
#include "HardwareAbstract.hpp"
#include "PageBase.hpp"

namespace UI::Widget {
class Label;
}

namespace UI::Page {

class SystemSettings : public Base {
public:
  SystemSettings(std::shared_ptr<HardwareAbstract> aHardware);

protected:
  std::string GetTitle() override { return "System Settings"; }

private:
  std::shared_ptr<HardwareAbstract> mHardware;
  Widget::Label *mTimeoutLabel;
  Widget::DropDown<int> *mScreenTimeOutDropDown;
};

} // namespace UI::Page