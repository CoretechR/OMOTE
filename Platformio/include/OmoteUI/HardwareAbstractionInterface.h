#pragma once
#include <lvgl.h>
#include <string>

class HardwareAbstractionInterface {
public:
  HardwareAbstractionInterface() = default;

  virtual void init() = 0;
  virtual void sendIR() = 0;
  virtual void MQTTPublish(const char *topic, const char *payload) = 0;
  virtual void debugPrint(std::string message) = 0;
};
