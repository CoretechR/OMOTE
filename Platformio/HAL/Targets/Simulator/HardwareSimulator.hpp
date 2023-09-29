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

  virtual void init() override{};

  virtual void debugPrint(const char *fmt, ...) override {
    va_list arguments;
    va_start(arguments, fmt);
    vprintf(fmt, arguments);
    va_end(arguments);
  }

  virtual std::shared_ptr<BatteryInterface> battery() override;
  virtual std::shared_ptr<DisplayAbstract> display() override;
  virtual std::shared_ptr<wifiHandlerInterface> wifi() override;
  virtual std::shared_ptr<KeyPressAbstract> keys() override;

  virtual char getCurrentDevice() override;
  virtual void setCurrentDevice(char currentDevice) override;

  virtual bool getWakeupByIMUEnabled() override;
  virtual void setWakeupByIMUEnabled(bool wakeupByIMUEnabled) override;

  virtual uint16_t getSleepTimeout() override;
  virtual void setSleepTimeout(uint16_t sleepTimeout) override;

private:
  std::thread mTickThread;
  std::thread mHardwareStatusTitleUpdate;

  std::shared_ptr<BatterySimulator> mBattery;
  std::shared_ptr<SDLDisplay> mDisplay;
  std::shared_ptr<wifiHandlerSim> mWifiHandler;
  std::shared_ptr<KeyPressSim> mKeys;
};
