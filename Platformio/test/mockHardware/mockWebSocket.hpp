#pragma once
#include "Hardware/websockets/webSocketInterface.hpp"

class MockWebSocket : public webSocketInterface {
 public:
  MockWebSocket() = default;
  explicit MockWebSocket(std::unique_ptr<Json::IProcessMessage> aJsonHandler)
      : webSocketInterface(std::move(aJsonHandler)) {}

  void connect(const std::string& url) override { Connected(); }

  void disconnect() override { Disconnected(); }

  void sendMessage(const std::string& message) override {
    mLastSentMessage = message;
  }

  void setMessageCallback(MessageCallback callback) override {
    mMessageCallback = std::move(callback);
  }

  // Test helper methods
  void simulateReceiveMessage(const std::string& message) {
    if (mMessageCallback) {
      mMessageCallback(message);
    }
  }

  const std::string& getLastSentMessage() const { return mLastSentMessage; }

 private:
  MessageCallback mMessageCallback;
  std::string mLastSentMessage;
};
