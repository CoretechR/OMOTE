#pragma once
#include "HardwareAbstract.hpp"

#include "batterySimulator.hpp"
#include "SDLDisplay.hpp"
#include "wifiHandlerSim.hpp"

class HardwareSimulator : public HardwareAbstract {
public:
  HardwareSimulator();
  
  virtual void init() override {};

  virtual void debugPrint(const char* fmt, ...) override {
    va_list arguments;
    va_start(arguments, fmt);
    vprintf(fmt, arguments);
    va_end(arguments);
  }

  virtual std::shared_ptr<BatteryInterface> battery() override; 
  virtual std::shared_ptr<DisplayAbstract> display() override;
  virtual std::shared_ptr<wifiHandlerInterface> wifi() override;

private:
  std::shared_ptr<BatterySimulator> mBattery;
  std::shared_ptr<SDLDisplay> mDisplay;
  std::shared_ptr<wifiHandlerSim> mWifiHandler;
};
