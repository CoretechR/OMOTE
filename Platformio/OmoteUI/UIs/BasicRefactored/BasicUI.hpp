#pragma once
#include "UIBase.hpp"

namespace UI {

class BasicUI : public UIBase {
public:
  BasicUI(std::shared_ptr<HardwareAbstract> aHardware);
};

} // namespace UI