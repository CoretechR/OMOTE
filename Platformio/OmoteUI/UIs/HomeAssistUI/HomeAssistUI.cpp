#include "UIs/HomeAssistUI/HomeAssistUI.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssistant/Api/WebSocket/Message/Attributes/Light.hpp"
#include "HomeAssistant/Api/WebSocket/Message/Entity.hpp"
#include "HomeAssistant/Api/WebSocket/Message/Message.hpp"
#include "HomeAssistant/Api/WebSocket/Message/MessageHandler.hpp"
#include "HomeAssistant/Api/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssistant/Api/WebSocket/Session/Session.hpp"
#include "UIs/HomeAssistUI/page/DeviceList.hpp"

using namespace UI;

using namespace HomeAssist::WebSocket;

HomeAssistUI::HomeAssistUI() : BasicUI() {
  auto& hardware = HardwareFactory::getAbstract();
  auto socket = hardware.webSocket();
  if (!socket) {
    hardware.debugPrint("Unable To Get WebSocket Total Failure Condition!");
    return;
  }
  mHomeAssistApi = std::make_unique<Api>(socket);
  AddPageToHomeScreen(std::make_unique<UI::Page::DeviceList>(*mHomeAssistApi));
};

void HomeAssistUI::loopHandler() {
  BasicUI::loopHandler();
  if (mHomeAssistApi) {
    mHomeAssistApi->Process();
  }
}
