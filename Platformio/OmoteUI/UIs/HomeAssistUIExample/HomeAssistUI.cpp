#include "HomeAssistUIExample/HomeAssistUI.hpp"

#include "DevicesQueryProcessor.hpp"
#include "HardwareFactory.hpp"
#include "HomeAssist/WebSocket/Message/Attributes/Light.hpp"
#include "HomeAssist/WebSocket/Message/Entity.hpp"
#include "HomeAssist/WebSocket/Message/Message.hpp"
#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"
#include "HomeAssist/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Session/Session.hpp"

using namespace UI;

using namespace HomeAssist::WebSocket;

HomeAssistUI::HomeAssistUI() : BasicUI() {
  auto socket = HardwareFactory::getAbstract().webSocket();
  if (!socket) {
    return;
  }

  mHomeAssistApi = std::make_unique<Api>(socket);
  mMessageHandler =
      std::make_shared<MessageHandler>([](const Message& aMessage) {
        if (auto* newState = aMessage.BorrowToState(); newState) {
          if (auto* attributes = newState->BorrowAttributes(); attributes) {
            if (auto* light = attributes->BorrowLight(); light) {
              auto [r, g, b] = light->GetRgb();
              HardwareFactory::getAbstract().debugPrint(
                  "Light Update in UI: Brightness:%d color:r:%dg:%db:%d \n",
                  light->GetBrightness(), r, g, b);
            }
          }
        }
        return true;
      });

  mDeviceFinder = std::make_shared<MessageHandler>([](const Message& aMessage) {
    for (const auto& entity : aMessage.BorrowEntityList()) {
      if (auto* attributes = entity->BorrowAttributes(); attributes) {
        if (auto* light = attributes->BorrowLight(); light) {
          HardwareFactory::getAbstract().debugPrint(
              "Id: %s State: %s Brightness:%d \n", entity->GetId().c_str(),
              entity->GetState().c_str(), light->GetBrightness());
        }
      }
    }
    return true;
  });

  mDeviceQueryProcessor = std::make_shared<UI::DevicesQueryProcessor>();

  auto lightRequest = std::make_unique<Request>(HomeAssist::TestSubEvent);
  auto stateRequest = std::make_unique<Request>(HomeAssist::GetStatesMessage);

  // mHomeAssistApi->AddSession(
  //     std::make_unique<Session>(std::move(lightRequest), mMessageHandler));

  mHomeAssistApi->AddSession(std::make_unique<Session>(
      std::move(stateRequest), nullptr, mDeviceQueryProcessor));
}

void HomeAssistUI::loopHandler() {
  BasicUI::loopHandler();
  if (mHomeAssistApi) {
    mHomeAssistApi->Process();
  }
}
