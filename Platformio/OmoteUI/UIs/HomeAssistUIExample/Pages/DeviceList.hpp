#pragma once
#include "HomeAssist/WebSocket/Api.hpp"
#include "core/page/PageBase.hpp"

namespace UI::Widget {
class List;
}

namespace UI::Page {

class DeviceList : public Base {
 public:
  DeviceList(HomeAssist::WebSocket::Api& aApi);
  virtual ~DeviceList() = default;

  // Override from Base
  std::string GetTitle() override { return "Devices"; }

 protected:
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

 private:
  void InitializeUI();
  Widget::List* mDeviceList;
  HomeAssist::WebSocket::Api& mApi;
};

}  // namespace UI::Page
