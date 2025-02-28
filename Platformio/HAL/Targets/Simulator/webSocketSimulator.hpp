#pragma once
#include <iostream>

#include "webSocketInterface.hpp"

class webSocketSimulator : public webSocketInterface {
 public:
  void connect(const std::string& url) override;
  void disconnect() override;
  void sendMessage(const std::string& message) override;
  void setMessageCallback(MessageCallback callback) override;
  bool isConnected() const override;

 private:
  bool connected = false;
  MessageCallback messageCallback;
};
