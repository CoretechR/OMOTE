// OMOTE Hardware Abstraction
// 2023 Matthew Colvin
#pragma once
#include "wifiHandlerInterface.h"
#include <functional>
#include <lvgl.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "Notification.hpp"

class HardwareAbstract {
public:
  HardwareAbstract(
  );

  struct batteryStatus {
      /// @brief Percent of battery remaining (0-100]
      int percentage;
      /// @brief True - Battery is Charging
      ///        False - Battery discharging
      bool isCharging;
  };
  virtual std::optional<batteryStatus> getBatteryStatus();

  /// @brief Register function to be ran when hardware notifies battery
  ///        status has changed.
  /// @param onBatteryStatusChangeHandler - Callable to be ran when batter status changes
  void onBatteryChange(std::function<void(batteryStatus)> onBatteryStatusChangeHandler);
  
  virtual std::shared_ptr<wifiHandlerInterface> wifi() = 0;
  /// @brief Override in order to do setup of hardware devices
  virtual void init() = 0;

  /// @brief Override to allow printing of a message for debugging
  /// @param message - Debug message
  virtual void debugPrint(const char* fmt, ...) = 0;

  protected:
    Notification<batteryStatus> mBatteryNotification;

  private:

};