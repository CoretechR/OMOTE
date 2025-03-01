#include "HomeAssist/WebSocket/WebSocketApi.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssist/WebSocket/Message/WebSocketMessage.hpp"
#include "HomeAssist/WebSocket/Message/WebSocketPredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Session/IWebSocketSession.hpp"
#include "RapidJsonUtilty.hpp"
#include "rapidjson/document.h"

namespace HomeAssist {
WebSocketApi::WebSocketApi(std::shared_ptr<webSocketInterface> socket)
    : mHomeAssistSocket(socket) {
  if (mHomeAssistSocket) {
    mHomeAssistSocket->connect("ws://homeassistant.local:8123/api/websocket");
    mHomeAssistSocket->setMessageCallback(
        [this](const std::string& messageStr) {
          ParseIncomingMessage(messageStr);
        });
  }
}

WebSocketApi::~WebSocketApi() {}

void WebSocketApi::ProccessMessages() {
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

void WebSocketApi::CleanUpSessions() {
  for (auto sessionIter = mSessions.begin(); sessionIter != mSessions.end();) {
    if (!*sessionIter) {
      sessionIter = mSessions.erase(sessionIter);
    } else {
      ++sessionIter;
    }
  }
}

bool WebSocketApi::PreProccessMessage(WebSocketMessage& aMessage) {
  if (mConnectionStatus == ConnectionStatus::Initializing) {
    switch (aMessage.GetType()) {
      case WebSocketMessage::Type::auth_required:
        mHomeAssistSocket->sendMessage(HomeAssistAuthResponse);
        return true;
      case WebSocketMessage::Type::auth_ok:
        mConnectionStatus = ConnectionStatus::Connected;
        mHomeAssistSocket->sendMessage(ConfigAreaRegistryList);
        return true;
      case WebSocketMessage::Type::auth_invalid:
        mConnectionStatus = ConnectionStatus::Failed;
        return true;
      default:
        return false;
    }
  }
  return false;
}

void WebSocketApi::ParseIncomingMessage(const std::string& messageStr) {
  rapidjson::Document messageJson;
  messageJson.Parse(messageStr.c_str());
  auto prettyDebugString = ToPrettyString(messageJson);
  HardwareFactory::getAbstract().debugPrint("%s", prettyDebugString.c_str());
  auto messageObj = std::make_unique<WebSocketMessage>(messageJson);
  if (PreProccessMessage(*messageObj)) {
    return;
  }
  mIncomingMessageQueue.push(std::move(messageObj));
}

}  // namespace HomeAssist