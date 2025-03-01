#include "HomeAssist/WebSocketApi.hpp"

#include "HardwareFactory.hpp"

namespace HomeAssist {

WebSocketApi::WebSocketApi(std::shared_ptr<webSocketInterface> socket)
    : mHomeAssistSocket(socket) {
  if (mHomeAssistSocket) {
    mHomeAssistSocket->connect("ws://homeassistant.local:8123/api/websocket");
    mHomeAssistSocket->setMessageCallback([](const std::string& message) {
      HardwareFactory::getAbstract().debugPrint("%s\n", message.c_str());
    });
  }
}

WebSocketApi::~WebSocketApi() {}

}  // namespace HomeAssist