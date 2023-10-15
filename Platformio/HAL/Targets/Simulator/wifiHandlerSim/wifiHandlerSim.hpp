#pragma once
#include "Notification.hpp"
#include "wifiHandlerInterface.h"
#include <memory>

class wifiHandlerSim : public wifiHandlerInterface {
public:
  wifiHandlerSim();

  /**
   * @brief Connect to the wifi using the provided credetials
   */
  void connect(std::string ssid, std::string password) override;

  /**
   * @brief function to trigger asynchronous scan for wifi networks
   */
  void scan();
  bool isAvailable();
  void begin();

private:
  wifiStatus status = wifiStatus(true, "172.0.0.1", "FakeNet");
};