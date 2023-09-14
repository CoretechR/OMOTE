// OMOTE Hardware Abstraction
// 2023 Matthew Colvin
#pragma once
#include "BatteryInterface.h"
#include "DisplayAbstract.h"
#include "wifiHandlerInterface.h"

#include "Notification.hpp"

#include <memory>

class HardwareAbstract {
public:
  HardwareAbstract();

  /// @brief Override in order to do setup of hardware devices post construction
  virtual void init() = 0;


  /// @brief Override to allow printing of a message for debugging
  /// @param message - Debug message
  virtual void debugPrint(const char* fmt, ...) = 0;

  virtual std::shared_ptr<BatteryInterface> battery() = 0; 
  virtual std::shared_ptr<DisplayAbstract> display() = 0;
  virtual std::shared_ptr<wifiHandlerInterface> wifi() = 0;
  
  protected:

};