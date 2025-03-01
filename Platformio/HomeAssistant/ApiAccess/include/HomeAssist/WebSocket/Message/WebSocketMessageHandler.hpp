#pragma once

#include <functional>
#include <memory>
#include <string>

#include "HomeAssist/WebSocket/Message/WebSocketMessage.hpp"

namespace HomeAssist {

class WebSocketMessageHandler {
 public:
  using MessageType = const WebSocketMessage&;
  using MessageProccessor = std::function<bool(MessageType)>;

  WebSocketMessageHandler(MessageProccessor aMessageProccessor);
  virtual ~WebSocketMessageHandler() = default;

  bool ProcessMessage(MessageType aMessageToProccess);

  void SetMessageProccessor(MessageProccessor aMessageProccessor);

 private:
  MessageProccessor mMessageProccessor;
};

inline WebSocketMessageHandler::WebSocketMessageHandler(
    MessageProccessor aMessageProccessor)
    : mMessageProccessor(aMessageProccessor) {}

inline bool WebSocketMessageHandler::ProcessMessage(
    MessageType aMessageToProccess) {
  return mMessageProccessor ? mMessageProccessor(aMessageToProccess) : false;
}

inline void WebSocketMessageHandler::SetMessageProccessor(
    MessageProccessor aMessageProccessor) {
  mMessageProccessor = aMessageProccessor;
}

}  // namespace HomeAssist
