#pragma once
#include "Notification.hpp"
#include "memory.h"
#include "wifiHandlerInterface.h"
#include <WiFi.h>

class wifiHandler : public wifiHandlerInterface {
public:
  wifiHandler();
  static std::shared_ptr<wifiHandler> getInstance();
  /**
   * @brief Function to initialize the wifi handler
   */
  void begin();

  /**
   * @brief Connect to the wifi using the provided credetials
   */
  void connect(std::string ssid, std::string password) override;

  /**
   * @brief function to trigger asynchronous scan for wifi networks
   */
  void scan();
  bool isAvailable();

private:
  /**
   * @brief Function to update the wifi credentials. This function is called in
   * the wifi event callback function after a connection is established. Only
   * then is the new credentials stored and the old stored credentials
   * overwritten.
   *
   * @param temporary_ssid
   * @param temporary_password
   */
  void update_credentials();

  void WiFiEvent(WiFiEvent_t event);

  /**
   * @brief Function to turn off wifi
   */
  void turnOff();
  /**
   * @brief Function to get the IP address of this device
   * @return String IP Address of the device
   */
  std::string getIP();
  static std::shared_ptr<wifiHandler> mInstance;
  bool connect_attempt = false;
  std::string temporary_password;
  std::string temporary_ssid;

  std::string password;
  std::string SSID;

  void update_status();

  /**
   * @brief Function to disconnect from the network
   */
  void disconnect();

  /**
   * @brief Function to determine wether or not we are connected to a network
   *
   * @return true  Device is connected to wifi network
   * @return false Device is not connected to wifi network
   */
  bool isConnected();
  /**
   * @brief Internal variable to store the wifi SSID
   *
   */
};