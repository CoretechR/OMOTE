// OMOTE Hardware Abstraction
// 2023 Matthew Colvin

#pragma once
#include <lvgl.h>
#include <string>
#include "BatteryInterface.h"

class HardwareInterface {
public:

  HardwareInterface() = default;

  virtual void init() = 0;
  virtual void sendIR() = 0;
  virtual void MQTTPublish(const char *topic, const char *payload) = 0;
  virtual BatteryInterface::batteryStatus getBatteryPercentage() = 0;
  virtual void debugPrint(std::string message) = 0;
};
