// OMOTE Hardware Abstraction
// 2023 Matthew Colvin

#pragma once
#include <functional>
#include <lvgl.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "BatteryInterface.h"
#include "DisplayInterface.h"
#include "wifiHandlerInterface.h"
#include "Notification.hpp"

class HardwareAbstract {
public:
  HardwareAbstract(std::shared_ptr<BatteryInterface> aBattery = nullptr,
                   std::shared_ptr<wifiHandlerInterface> aWifiHandler = nullptr,
                   std::shared_ptr<DisplayInterface> aDisplay = nullptr
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
  
  /// @brief Override in order to do setup of hardware devices
  virtual void init() = 0;
  
  /// @brief Override to allow printing of a message for debugging
  /// @param message - Debug message
  virtual void debugPrint(std::string message) = 0;

  protected:
    Notification<batteryStatus> mBatteryNotification;
    
  private:
    std::shared_ptr<BatteryInterface> mBattery;
    std::shared_ptr<wifiHandlerInterface> mWifiHandler;
    std::shared_ptr<DisplayInterface> mDisplay;
};
