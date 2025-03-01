#pragma once

#include "HomeAssist/WebSocket/Message/WebSocketMessage.hpp"
#include "HomeAssist/WebSocket/Message/WebSocketMessageHandler.hpp"
#include "HomeAssist/WebSocket/Session/IWebSocketSession.hpp"
#include "HomeAssist/WebSocket/WebSocketRequest.hpp"

namespace HomeAssist {

class WebSocketAuthSession : public IWebSocketSession {
 public:
  WebSocketAuthSession();

  bool ProcessMessage(const WebSocketMessage& aMessage) override {
    if (aMessage.GetType() == WebSocketMessage::Type::auth_required) {
      return true;
    } else if (aMessage.GetType() == WebSocketMessage::Type::auth_ok) {
      mIsAuthenticated = true;
      return true;
    } else if (aMessage.GetType() == WebSocketMessage::Type::auth_invalid) {
      mIsAuthenticated = false;
      return true;
    }
    return WebSocketSession::ProcessMessage(aMessage);
  }

  bool IsComplete() const override {
    return mIsAuthenticated || WebSocketSession::IsComplete();
  }

 private:
  bool mIsAuthenticated = false;
};

}  // namespace HomeAssist