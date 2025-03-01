#pragma once

#include <memory>

namespace HomeAssist::WebSocket {

class Message;
class MessageHandler;
class Request;

class Session {
 public:
  Session(std::unique_ptr<Request> aRequest,
          std::shared_ptr<MessageHandler> aMessageHandler);

  Request* BorrowStartRequest();
  Request* BorrowEndRequest();

  virtual bool ProcessMessage(const Message& aMessage);
  virtual bool IsComplete() const;

 protected:
  Session(std::shared_ptr<MessageHandler> aMessageHandler);

 private:
  std::unique_ptr<Request> mStartRequest = nullptr;
  std::unique_ptr<Request> mEndRequest = nullptr;
  std::weak_ptr<MessageHandler> mMessageHandler;
};

}  // namespace HomeAssist::WebSocket