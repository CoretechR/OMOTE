#include "DeviceList.hpp"

#include "HomeAssist/WebSocket/Message/PredefinedMessages.hpp"
#include "HomeAssist/WebSocket/Request.hpp"
#include "HomeAssist/WebSocket/Session/Session.hpp"
#include "List.hpp"
#include "UIElementIds.hpp"
#include "UIs/HomeAssistUIExample/DevicesQueryProcessor.hpp"

namespace UI::Page {

DeviceList::DeviceList(HomeAssist::WebSocket::Api& aApi)
    : Base(ID::Pages::HomeAssistDeviceList),
      mDeviceList(AddNewElement<Widget::List>()),
      mApi(aApi),
      mDeviceQueryProcessor(std::make_shared<UI::DevicesQueryProcessor>()) {
  auto entityRequest = std::make_unique<HomeAssist::WebSocket::Request>(
      HomeAssist::GetEntityMessage);

  mApi.AddSession(std::make_unique<HomeAssist::WebSocket::Session>(
      std::move(entityRequest), nullptr, mDeviceQueryProcessor));

  InitializeUI();
}

bool DeviceList::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return false;
}

void DeviceList::InitializeUI() {
  // Initialize UI elements here
}

}  // namespace UI::Page
