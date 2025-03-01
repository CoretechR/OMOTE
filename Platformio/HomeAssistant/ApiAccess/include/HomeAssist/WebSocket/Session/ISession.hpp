#pragma once

#include <memory>

namespace HomeAssist::WebSocket {

class Message;
class MessageHandler;
class Request;

class ISession {
 public:
  virtual ~ISession() = default;

  virtual Request* BorrowStartRequest() = 0;
  virtual Request* BorrowEndRequest() = 0;

  virtual bool ProcessMessage(const Message& aMessage) = 0;
  virtual bool IsComplete() const = 0;
};

}  // namespace HomeAssist::WebSocket