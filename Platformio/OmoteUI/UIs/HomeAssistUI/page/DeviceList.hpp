#pragma once
#include "Arc.hpp"
#include "HomeAssistant/Api/WebSocket/Api.hpp"
#include "HomeAssistant/Api/WebSocket/Message/MessageHandler.hpp"
#include "PageBase.hpp"
#include "UIs/HomeAssistUI/SessionProcessors/DevicesQueryProcessor.hpp"

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
  Widget::List* mDeviceList;
  Widget::Arc* mLoadingArc;
  HomeAssist::WebSocket::Api& mApi;
  std::shared_ptr<UI::DevicesQueryProcessor> mDeviceQueryProcessor;
};

}  // namespace UI::Page
