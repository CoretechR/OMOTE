#pragma once
#include <thread>

#include "HardwareAbstract.hpp"
#include "IRSim.hpp"
#include "KeyPressSim.hpp"
#include "SDLDisplay.hpp"
#include "StatsSimulator.hpp"
#include "batterySimulator.hpp"
#include "wifiHandlerSim.hpp"

class HardwareSimulator : public HardwareAbstract {
 public:
  HardwareSimulator();

  void init() override {};
  void loopHandler() override {};

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
  std::shared_ptr<IRInterface> ir() override;
  std::shared_ptr<SystemStatsInterface> stats() override;

  char getCurrentDevice() override;
  void setCurrentDevice(char currentDevice) override;

  bool getWakeupByIMUEnabled() override;
  void setWakeupByIMUEnabled(bool wakeupByIMUEnabled) override;

  uint16_t getSleepTimeout() override;
  void setSleepTimeout(uint16_t sleepTimeout) override;

 private:
  std::thread mHardwareStatusTitleUpdate;

  std::shared_ptr<BatterySimulator> mBattery;
  std::shared_ptr<SDLDisplay> mDisplay;
  std::shared_ptr<wifiHandlerSim> mWifiHandler;
  std::shared_ptr<KeyPressSim> mKeys;
  std::shared_ptr<IRSim> mIr;
  std::shared_ptr<StatsSimulator> mStats;
};
