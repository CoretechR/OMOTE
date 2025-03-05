#pragma once
#include <functional>
#include <string>

#include "websockets/IProcessMessage.hpp"

class webSocketInterface {
 public:
  using MessageCallback = std::function<void(const std::string&)>;

  webSocketInterface() = default;
  virtual ~webSocketInterface() = default;

  webSocketInterface(
      std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> aJsonHandler);

  void SetJsonHandler(
      std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> aJsonHandler);

  virtual bool isConnected() const = 0;
  virtual void connect(const std::string& url) = 0;
  virtual void disconnect() = 0;

  virtual void sendMessage(const std::string& message) = 0;
  virtual void setMessageCallback(MessageCallback callback) = 0;

 protected:
  std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> mJsonHandler = nullptr;
};

inline webSocketInterface::webSocketInterface(
    std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> aJsonHandler)
    : mJsonHandler(std::move(aJsonHandler)) {}

inline void webSocketInterface::SetJsonHandler(
    std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> aNewJsonHandler) {
  mJsonHandler = std::move(aNewJsonHandler);
}