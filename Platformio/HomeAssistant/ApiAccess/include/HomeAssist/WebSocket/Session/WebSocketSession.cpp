#include "HomeAssist/WebSocket/Session/WebSocketSession.hpp"

#include "HomeAssist/WebSocket/Message/WebSocketMessageHandler.hpp"
#include "HomeAssist/WebSocket/WebSocketRequest.hpp"

namespace HomeAssist::WebSocket {

WebSocketSession::WebSocketSession(
    std::unique_ptr<WebSocketRequest> aRequest,
    std::shared_ptr<WebSocketMessageHandler> aMessageHandler)
    : mStartRequest(std::move(aRequest)), mMessageHandler(aMessageHandler) {}

WebSocketSession::WebSocketSession(
    std::shared_ptr<WebSocketMessageHandler> aMessageHandler)
    : mMessageHandler(aMessageHandler) {}

bool WebSocketSession::ProcessMessage(const WebSocketMessage& aMessage) {
  if (auto handler = mMessageHandler.lock(); handler) {
    return handler->ProcessMessage(aMessage);
  }
  return false;
}

bool WebSocketSession::IsComplete() const {
  // Our message handler is no longer so we are now done with this session
  return mMessageHandler.use_count() == 0;
}

WebSocketRequest* WebSocketSession::BorrowStartRequest() {
  return mStartRequest.get();
}

WebSocketRequest* WebSocketSession::BorrowEndRequest() {
  return mEndRequest.get();
}

}  // namespace HomeAssist::WebSocket