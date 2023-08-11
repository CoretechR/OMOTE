#pragma once
#include "HardwareInterface.h"
#include <iostream>
#include <string>

class HardwareSimulator : public HardwareInterface {
public:
  HardwareSimulator() : HardwareInterface(nullptr){};

  virtual void debugPrint(std::string message) override {
    std::cout << message;
  }

  virtual void sendIR() override {}

  virtual void MQTTPublish(const char *topic, const char *payload) override{

  };

  virtual void init() override;

  virtual std::optional<HardwareInterface::batteryStatus> getBatteryStatus() override {
    HardwareInterface::batteryStatus fakeStatus;
    fakeStatus.isCharging = false;
    fakeStatus.percentage = 100;
    return fakeStatus;
  }
};
