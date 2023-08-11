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

  virtual void init() = 0;
  virtual void sendIR() = 0;
  virtual void MQTTPublish(const char *topic, const char *payload) = 0;
  virtual void debugPrint(std::string message) = 0;

  private:
    std::shared_ptr<BatteryInterface> mBattery;
};
