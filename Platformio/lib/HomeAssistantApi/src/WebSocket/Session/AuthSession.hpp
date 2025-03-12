#pragma once

#include "HardwareFactory.hpp"
#include "WebSocket/Api.hpp"
#include "WebSocket/Message/Message.hpp"
#include "WebSocket/Message/MessageHandler.hpp"
#include "WebSocket/Request.hpp"
#include "WebSocket/Session/ISession.hpp"

namespace HomeAssist::WebSocket {

static const auto HomeAssistAuthResponse = R"---(
    {
      "type": "auth",
      "access_token": ")---" + std::string(HOMEASSISTANT_API_TOKEN) +
                                           "\"}";

class AuthSession : public ISession {
 public:
  AuthSession(std::shared_ptr<webSocketInterface> aHomeAssistSocket);
  ~AuthSession() override = default;

  bool ProcessMessage(const Message& aMessage) override;

  bool IsComplete() const override;

  Api::ConnectionStatus GetConnectionStatus() const;

  void SendAuth();

  bool IsAuthSent();

 private:
  Request* BorrowEndRequest() override { return nullptr; }
  Request* BorrowStartRequest() override { return nullptr; }
  std::shared_ptr<Json::IChunkProcessor> GetChunkProcessor() override {
    return nullptr;
  }
  bool IsPreferringChunkProcessing() override { return false; }

  std::shared_ptr<webSocketInterface> mHomeAssitSocket = nullptr;
  Api::ConnectionStatus mConnectionStatus = Api::ConnectionStatus::Initializing;
  bool mIsAuthSent = false;
};

AuthSession::AuthSession(std::shared_ptr<webSocketInterface> aHomeAssistSocket)
    : mHomeAssitSocket(aHomeAssistSocket) {}

Api::ConnectionStatus AuthSession::GetConnectionStatus() const {
  return mConnectionStatus;
}

bool AuthSession::ProcessMessage(const Message& aMessage) {
  switch (aMessage.GetType()) {
    case Message::Type::auth_required:
      SendAuth();
      return true;
    case Message::Type::auth_ok:
      mConnectionStatus = Api::ConnectionStatus::Connected;
      HardwareFactory::getAbstract().debugPrint("HOLY_SHIT Connected");
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

void AuthSession::SendAuth() {
  mHomeAssitSocket->sendMessage(HomeAssistAuthResponse);
  mIsAuthSent = true;
}

bool AuthSession::IsAuthSent() { return mIsAuthSent; }

}  // namespace HomeAssist::WebSocket