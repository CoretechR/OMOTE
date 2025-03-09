#pragma once
#include <functional>
#include <string>

#include "IProcessMessage.hpp"

class webSocketInterface {
 public:
  using MessageCallback = std::function<void(const std::string&)>;

  webSocketInterface() = default;
  virtual ~webSocketInterface() = default;

  webSocketInterface(
      std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> aJsonHandler);

  void SetJsonHandler(std::unique_ptr<HAL::WebSocket::Json::IProcessMessage>
                          aJsonHandler = nullptr);

  bool isConnected() const;
  virtual void connect(const std::string& url) = 0;
  virtual void disconnect() = 0;

  virtual void sendMessage(const std::string& message) = 0;
  virtual void setMessageCallback(MessageCallback callback) = 0;

  virtual void OnConnect(std::function<void()> aConnectHandler = nullptr);
  virtual void OnDisconnect(std::function<void()> aDisconnectHandler = nullptr);

 protected:
  std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> mJsonHandler = nullptr;

  void Connected();
  void Disconnected();

 private:
  std::function<void()> mConnectedCallable;
  std::function<void()> mDisconnectedCallable;
  bool mIsConnected = false;
};

inline webSocketInterface::webSocketInterface(
    std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> aJsonHandler)
    : mJsonHandler(std::move(aJsonHandler)) {}

inline void webSocketInterface::SetJsonHandler(
    std::unique_ptr<HAL::WebSocket::Json::IProcessMessage> aNewJsonHandler) {
  mJsonHandler = std::move(aNewJsonHandler);
}

inline void webSocketInterface::OnConnect(
    std::function<void()> aConnectHandler) {
  mConnectedCallable = std::move(aConnectHandler);
}

inline void webSocketInterface::OnDisconnect(
    std::function<void()> aDisconnectHandler) {
  mDisconnectedCallable = std::move(aDisconnectHandler);
}

inline bool webSocketInterface::isConnected() const { return mIsConnected; }

inline void webSocketInterface::Connected() {
  if (mConnectedCallable) {
    mConnectedCallable();
  }
  mIsConnected = true;
}

inline void webSocketInterface::Disconnected() {
  if (mDisconnectedCallable) {
    mDisconnectedCallable();
  }
  mIsConnected = false;
}