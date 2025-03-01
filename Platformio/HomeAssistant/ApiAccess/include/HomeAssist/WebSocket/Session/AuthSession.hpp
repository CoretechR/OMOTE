#pragma once

#include "HomeAssist/WebSocket/Message/Message.hpp"
#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"
#include "HomeAssist/WebSocket/Request.hpp"
#include "HomeAssist/WebSocket/Session/ISession.hpp"

namespace HomeAssist::WebSocket {

class AuthSession : public ISession {
 public:
  AuthSession();

  bool ProcessMessage(const Message& aMessage) override {
    if (aMessage.GetType() == Message::Type::auth_required) {
      return true;
    } else if (aMessage.GetType() == Message::Type::auth_ok) {
      mIsAuthenticated = true;
      return true;
    } else if (aMessage.GetType() == Message::Type::auth_invalid) {
      mIsAuthenticated = false;
      return true;
    }
    return Session::ProcessMessage(aMessage);
  }

  bool IsComplete() const override {
    return mIsAuthenticated || Session::IsComplete();
  }

 private:
  bool mIsAuthenticated = false;
};

}  // namespace HomeAssist::WebSocket