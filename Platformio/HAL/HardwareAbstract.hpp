// OMOTE Hardware Abstraction
// 2023 Matthew Colvin

#pragma once
#include <lvgl.h>
#include <memory>
#include <optional>
#include <string>
#include "BatteryInterface.h"

class HardwareAbstract {
public:

  struct batteryStatus {
      /// @brief Percent of battery remaining (0-100]
      int percentage;
      /// @brief True - Battery is Charging
      ///        False - Battery discharging
      bool isCharging;
  };
  virtual std::optional<batteryStatus> getBatteryStatus();

  HardwareAbstract(std::shared_ptr<BatteryInterface> aBattery = nullptr);

  /// @brief Override in order to do setup of hardware devices
  virtual void init() = 0;
  
  /// @brief Override to allow printing of a message for debugging
  /// @param message - Debug message
  virtual void debugPrint(std::string message) = 0;

  private:
    std::shared_ptr<BatteryInterface> mBattery;
};
