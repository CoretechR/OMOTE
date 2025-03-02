#pragma once

#include <memory>

#include "HomeAssist/WebSocket/Request.hpp"
#include "HomeAssist/WebSocket/Session/ISession.hpp"

namespace HomeAssist::WebSocket {

class Message;
class MessageHandler;
class Request;

class Session : public ISession {
 public:
  Session(std::unique_ptr<Request> aRequest,
          std::shared_ptr<MessageHandler> aMessageHandler);

  Request* BorrowStartRequest() override;
  Request* BorrowEndRequest() override;

  bool ProcessMessage(const Message& aMessage) override;
  bool IsComplete() const override;

 private:
  std::unique_ptr<Request> mStartRequest = nullptr;
  std::unique_ptr<Request> mEndRequest = nullptr;
  std::weak_ptr<MessageHandler> mMessageHandler;
};

}  // namespace HomeAssist::WebSocket