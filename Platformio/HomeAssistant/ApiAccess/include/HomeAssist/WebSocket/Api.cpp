#include "HardwareFactory.hpp"
#include "HomeAssist/WebSocket/Api.hpp"
#include "HomeAssist/WebSocket/Message/Message.hpp"
#include "HomeAssist/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Session/ISession.hpp"
#include "RapidJsonUtilty.hpp"
#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

Api::Api(std::shared_ptr<webSocketInterface> socket)
    : mHomeAssistSocket(socket) {
  if (mHomeAssistSocket) {
    mHomeAssistSocket->connect("ws://homeassistant.local:8123/api/websocket");
    mHomeAssistSocket->setMessageCallback(
        [this](const std::string& messageStr) {
          ParseIncomingMessage(messageStr);
        });
  }
}

Api::~Api() {}

void Api::ProccessMessages() {
  while (mIncomingMessageQueue.size() > 0) {
    auto message = std::move(mIncomingMessageQueue.front());
    mIncomingMessageQueue.pop();
    for (auto& session : mSessions) {
      if (session->ProcessMessage(*message)) {
        break;
      } else if (session->IsComplete()) {
        session = nullptr;
      }
    }
  }
  CleanUpSessions();
}

void Api::CleanUpSessions() {
  for (auto sessionIter = mSessions.begin(); sessionIter != mSessions.end();) {
    if (!*sessionIter) {
      sessionIter = mSessions.erase(sessionIter);
    } else {
      ++sessionIter;
    }
  }
}

bool Api::PreProccessMessage(Message& aMessage) {
  if (mConnectionStatus == ConnectionStatus::Initializing) {
    switch (aMessage.GetType()) {
      case Message::Type::auth_required:
        mHomeAssistSocket->sendMessage(HomeAssistAuthResponse);
        return true;
      case Message::Type::auth_ok:
        mConnectionStatus = ConnectionStatus::Connected;
        mHomeAssistSocket->sendMessage(ConfigAreaRegistryList);
        return true;
      case Message::Type::auth_invalid:
        mConnectionStatus = ConnectionStatus::Failed;
        return true;
      default:
        return false;
    }
  }
  return false;
}

void Api::ParseIncomingMessage(const std::string& messageStr) {
  rapidjson::Document messageJson;
  messageJson.Parse(messageStr.c_str());
  auto prettyDebugString = ToPrettyString(messageJson);
  HardwareFactory::getAbstract().debugPrint("%s", prettyDebugString.c_str());
  auto messageObj = std::make_unique<Message>(messageJson);
  if (PreProccessMessage(*messageObj)) {
    return;
  }
  mIncomingMessageQueue.push(std::move(messageObj));
}

}  // namespace HomeAssist::WebSocket