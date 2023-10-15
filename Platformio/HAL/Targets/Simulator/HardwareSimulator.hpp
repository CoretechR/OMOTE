#pragma once
#include "HardwareAbstract.hpp"

#include "KeyPressSim.hpp"
#include "SDLDisplay.hpp"
#include "batterySimulator.hpp"
#include "wifiHandlerSim.hpp"

#include <thread>

class HardwareSimulator : public HardwareAbstract {
public:
  HardwareSimulator();

  void init() override{};
  void loopHandler() override{};

  void debugPrint(const char *fmt, ...) override {
    va_list arguments;
    va_start(arguments, fmt);
    vprintf(fmt, arguments);
    va_end(arguments);
  }

  std::shared_ptr<BatteryInterface> battery() override;
  std::shared_ptr<DisplayAbstract> display() override;
  std::shared_ptr<wifiHandlerInterface> wifi() override;
  std::shared_ptr<KeyPressAbstract> keys() override;

  char getCurrentDevice() override;
  void setCurrentDevice(char currentDevice) override;

  bool getWakeupByIMUEnabled() override;
  void setWakeupByIMUEnabled(bool wakeupByIMUEnabled) override;

  uint16_t getSleepTimeout() override;
  void setSleepTimeout(uint16_t sleepTimeout) override;

private:
  std::thread mTickThread;
  std::thread mHardwareStatusTitleUpdate;

  std::shared_ptr<BatterySimulator> mBattery;
  std::shared_ptr<SDLDisplay> mDisplay;
  std::shared_ptr<wifiHandlerSim> mWifiHandler;
  std::shared_ptr<KeyPressSim> mKeys;
};
