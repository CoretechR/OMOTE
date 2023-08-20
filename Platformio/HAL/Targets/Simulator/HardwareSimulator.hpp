#pragma once
#include "HardwareAbstract.hpp"
#include <iostream>
#include <string>

class HardwareSimulator : public HardwareAbstract {
public:
  HardwareSimulator();
#if 0
  virtual void debugPrint(std::string message) override {
    std::cout << message;
  }
  #else
  virtual void debugPrint(const char* fmt, ...) override {
    va_list arguments;
    va_start(arguments, fmt);
    vprintf(fmt, arguments);
    va_end(arguments);
  }
  #endif

  virtual void init() override {};

  virtual std::optional<HardwareAbstract::batteryStatus> getBatteryStatus() override {
    HardwareAbstract::batteryStatus fakeStatus;
    fakeStatus.isCharging = false;
    fakeStatus.percentage = 100;
    return fakeStatus;
  }
};
