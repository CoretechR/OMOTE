#pragma once
#include "BasicUI.hpp"
#include "HomeAssistant/Api/WebSocket/Api.hpp"
#include "HomeAssistant/Api/WebSocket/Message/MessageHandler.hpp"

namespace UI {

class HomeAssistUI : public BasicUI {
 public:
  HomeAssistUI();

  void loopHandler() override;

 private:
  std::unique_ptr<HomeAssist::WebSocket::Api> mHomeAssistApi = nullptr;
};

}  // namespace UI
