// OMOTE Hardware Abstraction
// 2023 Matthew Colvin

#pragma once
#include <lvgl.h>
#include <memory>
#include <optional>
#include <string>
#include "BatteryInterface.h"

class HardwareInterface {
public:

  HardwareInterface(std::shared_ptr<BatteryInterface> aBattery);

  virtual void init() = 0;
  virtual void sendIR() = 0;
  virtual void MQTTPublish(const char *topic, const char *payload) = 0;
  virtual void debugPrint(std::string message) = 0;
  
  virtual std::optional<BatteryInterface::batteryStatus> getBatteryStatus() {
    if(mBattery){
      return mBattery->getBatteryPercentage();
    }
    return std::nullopt;
  }

  private:
    std::shared_ptr<BatteryInterface> mBattery;
};
