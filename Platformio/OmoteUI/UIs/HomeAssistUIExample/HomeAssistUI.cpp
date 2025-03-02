#include "HomeAssistUIExample/HomeAssistUI.hpp"

#include "HardwareFactory.hpp"
#include "HomeAssist/WebSocket/Message/Attributes/Light.hpp"
#include "HomeAssist/WebSocket/Message/Message.hpp"
#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"
#include "HomeAssist/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Message/State.hpp"
#include "HomeAssist/WebSocket/Session/Session.hpp"

using namespace UI;

using namespace HomeAssist::WebSocket;

HomeAssistUI::HomeAssistUI()
    : BasicUI(),
      mHomeAssistApi(
          std::make_unique<Api>(HardwareFactory::getAbstract().webSocket())) {
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

  auto request = std::make_unique<Request>(HomeAssist::TestSubEvent);

  mHomeAssistApi->AddSession(
      std::make_unique<Session>(std::move(request), mMessageHandler));
}

void HomeAssistUI::loopHandler() {
  BasicUI::loopHandler();
  mHomeAssistApi->Proccess();
}
