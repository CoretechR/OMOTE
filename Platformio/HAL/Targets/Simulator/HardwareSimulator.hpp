#pragma once
#include "HardwareAbstract.hpp"
#include "wifiHandlerSim.hpp"
#include <iostream>
#include <string>
#include "SDLDisplay.hpp"

class HardwareSimulator : public HardwareAbstract {
public:
  HardwareSimulator();
  virtual void debugPrint(const char* fmt, ...) override {
    va_list arguments;
    va_start(arguments, fmt);
    vprintf(fmt, arguments);
    va_end(arguments);
  }

  std::shared_ptr<wifiHandlerInterface> wifi();

  virtual void init() override {};

  virtual std::optional<HardwareAbstract::batteryStatus> getBatteryStatus() override {
    HardwareAbstract::batteryStatus fakeStatus;
    fakeStatus.isCharging = false;
    fakeStatus.percentage = 100;
    return fakeStatus;
  }
private:
  std::shared_ptr<wifiHandlerSim> mWifiHandler;
  std::shared_ptr<SDLDisplay> mDisplay;
};
