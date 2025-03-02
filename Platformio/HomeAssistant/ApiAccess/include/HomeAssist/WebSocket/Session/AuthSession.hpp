#pragma once

#include "HomeAssist/WebSocket/Api.hpp"
#include "HomeAssist/WebSocket/Message/Message.hpp"
#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"
#include "HomeAssist/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Request.hpp"
#include "HomeAssist/WebSocket/Session/ISession.hpp"

namespace HomeAssist::WebSocket {

class AuthSession : public ISession {
 public:
  AuthSession(std::shared_ptr<webSocketInterface> aHomeAssistSocket);
  ~AuthSession() override = default;

  bool ProcessMessage(const Message& aMessage) override;

  bool IsComplete() const override;

  Api::ConnectionStatus GetConnectionStatus() const;

 private:
  Request* BorrowEndRequest() override { return nullptr; }
  Request* BorrowStartRequest() override { return nullptr; }

  std::shared_ptr<webSocketInterface> mHomeAssitSocket = nullptr;
  Api::ConnectionStatus mConnectionStatus = Api::ConnectionStatus::Initializing;
};

AuthSession::AuthSession(std::shared_ptr<webSocketInterface> aHomeAssistSocket)
    : mHomeAssitSocket(aHomeAssistSocket) {}

Api::ConnectionStatus AuthSession::GetConnectionStatus() const {
  return mConnectionStatus;
}

bool AuthSession::ProcessMessage(const Message& aMessage) {
  switch (aMessage.GetType()) {
    case Message::Type::auth_required:
      mHomeAssitSocket->sendMessage(HomeAssistAuthResponse);
      return true;
    case Message::Type::auth_ok:
      mConnectionStatus = Api::ConnectionStatus::Connected;
      return true;
    case Message::Type::auth_invalid:
      mConnectionStatus = Api::ConnectionStatus::Failed;
      return true;
    default:
      return false;
  }
}

bool AuthSession::IsComplete() const {
  return mHomeAssitSocket == nullptr ||
         mConnectionStatus == Api::ConnectionStatus::Connected;
}

}  // namespace HomeAssist::WebSocket