// OMOTE UI
// 2023 Matthew Colvin

#pragma once
#include "HardwareAbstract.hpp"
#include <memory>

namespace UI {

class UIBase {
public:
  UIBase(std::shared_ptr<HardwareAbstract> aHardware);

protected:
  std::shared_ptr<HardwareAbstract> mHardware;
};

} // namespace UI