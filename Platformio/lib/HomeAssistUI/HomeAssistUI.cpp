#include "HomeAssistUI.hpp"

#include "DeviceList.hpp"
#include "HardwareFactory.hpp"
#include "WebSocket/Message/Attributes/Light.hpp"
#include "WebSocket/Message/Entity.hpp"
#include "WebSocket/Message/Message.hpp"
#include "WebSocket/Message/MessageHandler.hpp"
#include "WebSocket/Session/Session.hpp"

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
  mConnectionStatusHandler.SetNotification(
      mHomeAssistApi->GetConnectionNotification());
  mConnectionStatusHandler = [this](auto aStatus) {
    HandleConnectionStatusChange(aStatus);
  };

  AddPageToHomeScreen(std::make_unique<UI::Page::DeviceList>(*mHomeAssistApi));
};

void HomeAssistUI::HandleConnectionStatusChange(
    Api::ConnectionStatus aNewStatus) {
  if (aNewStatus == Api::ConnectionStatus::Disconnected) {
    // MatthewColvin/OMOTE#7
  }
}

void HomeAssistUI::loopHandler() {
  BasicUI::loopHandler();
  if (mHomeAssistApi) {
    mHomeAssistApi->Process();
  }
}
