#include "HomeAssist/WebSocketApi.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssist/WebSocketMessage.hpp"
#include "rapidjson/document.h"

static const auto HomeAssistAuthResponse = R"---(
{
  "type": "auth",
  "access_token": ")---" + std::string(HOMEASSISTANT_API_TOKEN) +
                                           "\"}";

namespace HomeAssist {
WebSocketApi::WebSocketApi(std::shared_ptr<webSocketInterface> socket)
    : mHomeAssistSocket(socket) {
  if (mHomeAssistSocket) {
    mHomeAssistSocket->connect("ws://homeassistant.local:8123/api/websocket");
    mHomeAssistSocket->setMessageCallback(
        [this](const std::string& messageStr) {
          HardwareFactory::getAbstract().debugPrint("%s\n", messageStr.c_str());
          ParseIncomingMessage(messageStr);
        });
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
  auto messageObj = std::make_unique<WebSocketMessage>(messageJson);
  if (PreProccessMessage(*messageObj)) {
    return;
  }
  mIncomingMessageQueue.push(std::move(messageObj));
}

WebSocketApi::~WebSocketApi() {}

}  // namespace HomeAssist