#pragma once

#include <functional>
#include <memory>
#include <string>

#include "WebSocket/Message/Message.hpp"

namespace HomeAssist::WebSocket {

class MessageHandler {
 public:
  using MessageType = const Message&;
  using MessageProccessor = std::function<bool(MessageType)>;

  MessageHandler(MessageProccessor aMessageProccessor);
  virtual ~MessageHandler() = default;

  bool ProcessMessage(MessageType aMessageToProccess);

  void SetMessageProccessor(MessageProccessor aMessageProccessor);

 private:
  MessageProccessor mMessageProccessor;
};

inline MessageHandler::MessageHandler(MessageProccessor aMessageProccessor)
    : mMessageProccessor(aMessageProccessor) {}

inline bool MessageHandler::ProcessMessage(MessageType aMessageToProccess) {
  return mMessageProccessor ? mMessageProccessor(aMessageToProccess) : false;
}

inline void MessageHandler::SetMessageProccessor(
    MessageProccessor aMessageProccessor) {
  mMessageProccessor = aMessageProccessor;
}

}  // namespace HomeAssist::WebSocket
