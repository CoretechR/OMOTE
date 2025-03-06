#include "DeviceList.hpp"
#include "List.hpp"
#include "UIElementIds.hpp"

namespace UI::Page {

DeviceList::DeviceList(HomeAssist::WebSocket::Api& aApi)
    : Base(ID::Pages::HomeAssistDeviceList),
      mDeviceList(AddNewElement<Widget::List>()),
      mApi(aApi) {
  InitializeUI();
}

bool DeviceList::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  return false;
}

void DeviceList::InitializeUI() {
  // Initialize UI elements here
}

}  // namespace UI::Page
