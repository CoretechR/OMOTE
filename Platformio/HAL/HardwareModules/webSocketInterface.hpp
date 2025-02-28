#pragma once
#include <functional>
#include <string>

class webSocketInterface {
 public:
  using MessageCallback = std::function<void(const std::string&)>;
  virtual ~webSocketInterface() = default;

  virtual void connect(const std::string& url) = 0;
  virtual void disconnect() = 0;
  virtual void sendMessage(const std::string& message) = 0;
  virtual void setMessageCallback(MessageCallback callback) = 0;
  virtual bool isConnected() const = 0;
};
