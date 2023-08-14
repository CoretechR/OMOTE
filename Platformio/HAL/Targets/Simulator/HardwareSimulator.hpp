#pragma once
#include "HardwareAbstract.hpp"
#include <iostream>
#include <string>

class HardwareSimulator : public HardwareAbstract {
public:
  HardwareSimulator();

  virtual void debugPrint(std::string message) override {
    std::cout << message;
  }

  virtual void init() override {};

  virtual std::optional<HardwareAbstract::batteryStatus> getBatteryStatus() override {
    HardwareAbstract::batteryStatus fakeStatus;
    fakeStatus.isCharging = false;
    fakeStatus.percentage = 100;
    return fakeStatus;
  }
};
