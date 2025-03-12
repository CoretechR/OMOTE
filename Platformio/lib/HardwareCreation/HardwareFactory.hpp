#pragma once
#include <memory>

#include "HardwareAbstract.hpp"
/**
 * @brief The HardwareFactory is responsible for making the
 */
class HardwareFactory {
 public:
  static void Init();

  static HardwareAbstract &getAbstract();

  static std::unique_ptr<HardwareAbstract> mHardware;
};
