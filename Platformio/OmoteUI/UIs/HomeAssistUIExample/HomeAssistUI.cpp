#include "HomeAssistUIExample/HomeAssistUI.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssist/WebSocket/Message/Attributes/Light.hpp"
#include "HomeAssist/WebSocket/Message/Entity.hpp"
#include "HomeAssist/WebSocket/Message/Message.hpp"
#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"
#include "HomeAssist/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Session/Session.hpp"
#include "HomeAssistUIExample/Pages/DeviceList.hpp"

using namespace UI;

using namespace HomeAssist::WebSocket;

HomeAssistUI::HomeAssistUI() : BasicUI() {
  auto socket = HardwareFactory::getAbstract().webSocket();
  if (!socket) {
    return;
  }

  mHomeAssistApi = std::make_unique<Api>(socket);

  AddPageToHomeScreen(std::make_unique<UI::Page::DeviceList>(*mHomeAssistApi));

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
}

void HomeAssistUI::loopHandler() {
  BasicUI::loopHandler();
  if (mHomeAssistApi) {
    mHomeAssistApi->Process();
  }
}
