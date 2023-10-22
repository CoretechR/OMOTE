// OMOTE Hardware Abstraction
// 2023 Matthew Colvin
#pragma once
#include "BatteryInterface.h"
#include "DisplayAbstract.h"
#include "KeyPressAbstract.hpp"
#include "wifiHandlerInterface.h"

#include "Notification.hpp"

#include <memory>

class HardwareAbstract {
public:
  HardwareAbstract();

  /// @brief Override in order to do setup of hardware devices post construction
  virtual void init() = 0;

  /// @brief Override to processing in main thread
  virtual void loopHandler() = 0;

  /// @brief Override to allow printing of a message for debugging
  /// @param message - Debug message
  virtual void debugPrint(const char *fmt, ...) = 0;

  virtual std::shared_ptr<BatteryInterface> battery() = 0;
  virtual std::shared_ptr<DisplayAbstract> display() = 0;
  virtual std::shared_ptr<wifiHandlerInterface> wifi() = 0;
  virtual std::shared_ptr<KeyPressAbstract> keys() = 0;

  virtual char getCurrentDevice() = 0;
  virtual void setCurrentDevice(char currentDevice) = 0;

  virtual bool getWakeupByIMUEnabled() = 0;
  virtual void setWakeupByIMUEnabled(bool wakeupByIMUEnabled) = 0;

  virtual uint16_t getSleepTimeout() = 0;
  virtual void setSleepTimeout(uint16_t sleepTimeout) = 0;
};