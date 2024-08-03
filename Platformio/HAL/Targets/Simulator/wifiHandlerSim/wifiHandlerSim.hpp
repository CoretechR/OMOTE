#pragma once
#include "Notification.hpp"
#include "wifiHandlerInterface.h"
#include <atomic>
#include <memory>
#include <thread>

class wifiHandlerSim : public wifiHandlerInterface {
public:
  wifiHandlerSim();
  ~wifiHandlerSim() override = default;

  void begin() override;
  void scan() override;
  void connect(std::string ssid, std::string password) override;
  wifiStatus GetStatus() override { return mCurrentStatus; };

  void setupMqttBroker(std::string aBrokerIpAddress,
                       int aPort = 1883) override{};
  void mqttSend(std::string aTopic, std::string aMessage) override{};

private:
  std::thread mFakeScanThread = std::thread([] {});
  std::thread mFakeStatusThread = std::thread([] {});
  wifiStatus mCurrentStatus = wifiStatus(true, "172.0.0.1", "FakeNet");
};