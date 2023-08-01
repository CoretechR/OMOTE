// OMOTE Hardware Abstraction
// 2023 Matthew Colvin

#pragma once
#include <lvgl.h>
#include <string>

class HardwareInterface {
public:
  struct batteryStatus {
    /// @brief Percent of battery remaining (0-100]
    int percentage;
    /// @brief Voltage of battery in millivolts
    int voltage;
    /// @brief True - Battery is Charging
    ///        False - Battery discharging
    bool isCharging;
  };

  HardwareInterface() = default;

  virtual void init() = 0;
  virtual void sendIR() = 0;
  virtual void MQTTPublish(const char *topic, const char *payload) = 0;
  virtual batteryStatus getBatteryPercentage() = 0;
  virtual void debugPrint(std::string message) = 0;
};
