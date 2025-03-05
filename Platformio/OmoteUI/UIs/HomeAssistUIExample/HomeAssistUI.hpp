#pragma once
#include "BasicUI.hpp"
#include "HomeAssist/WebSocket/Api.hpp"
#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"

namespace UI {

class HomeAssistUI : public BasicUI {
 public:
  HomeAssistUI();

  void loopHandler() override;

 private:
  std::unique_ptr<HomeAssist::WebSocket::Api> mHomeAssistApi = nullptr;
  std::shared_ptr<HomeAssist::WebSocket::MessageHandler> mMessageHandler;
  std::shared_ptr<HomeAssist::WebSocket::MessageHandler> mDeviceFinder;
  std::shared_ptr<HAL::WebSocket::Json::IChunkProcessor> mDeviceQueryProcessor;
};

}  // namespace UI
