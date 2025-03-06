#pragma once
#include "HomeAssist/WebSocket/Api.hpp"
#include "HomeAssist/WebSocket/Message/MessageHandler.hpp"
#include "core/page/PageBase.hpp"

namespace HAL::WebSocket::Json {
class IChunkProcessor;
}

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

  void AddEntity(const std::string& aEntity);

 private:
  void InitializeUI();
  Widget::List* mDeviceList;
  HomeAssist::WebSocket::Api& mApi;
  std::shared_ptr<HAL::WebSocket::Json::IChunkProcessor> mDeviceQueryProcessor;
};

}  // namespace UI::Page
