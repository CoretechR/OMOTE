#include "HomeAssist/WebSocket/Session/Session.hpp"

#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"
#include "HomeAssist/WebSocket/Request.hpp"

namespace HomeAssist::WebSocket {

Session::Session(std::unique_ptr<Request> aRequest,
                 std::shared_ptr<MessageHandler> aMessageHandler)
    : mStartRequest(std::move(aRequest)), mMessageHandler(aMessageHandler) {}

bool Session::ProcessMessage(const Message& aMessage) {
  if (auto handler = mMessageHandler.lock(); handler) {
    return handler->ProcessMessage(aMessage);
  }
  return false;
}

bool Session::IsComplete() const {
  // Our message handler is no longer so we are now done with this session
  return mMessageHandler.use_count() == 0;
}

Request* Session::BorrowStartRequest() { return mStartRequest.get(); }

Request* Session::BorrowEndRequest() { return mEndRequest.get(); }

}  // namespace HomeAssist::WebSocket