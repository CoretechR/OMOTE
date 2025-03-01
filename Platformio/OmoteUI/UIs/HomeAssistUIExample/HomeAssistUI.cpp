#include "HomeAssistUIExample/HomeAssistUI.hpp"

#include "HardwareFactory.hpp"

using namespace UI;

HomeAssistUI::HomeAssistUI()
    : BasicUI(),
      mHomeAssistSock(std::make_unique<HomeAssist::WebSocket::WebSocketApi>(
          HardwareFactory::getAbstract().webSocket())) {}
