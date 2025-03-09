#pragma once
#include "BasicUI.hpp"
#include "Notification.hpp"
#include "WebSocket/Api.hpp"
#include "WebSocket/Message/MessageHandler.hpp"
namespace UI {

class HomeAssistUI : public BasicUI {
 public:
  HomeAssistUI();

  void loopHandler() override;

 private:
  using ApiConnStatus = HomeAssist::WebSocket::Api::ConnectionStatus;
  Handler<ApiConnStatus> mConnectionStatusHandler;
  void HandleConnectionStatusChange(ApiConnStatus aNewStatus);

  std::unique_ptr<HomeAssist::WebSocket::Api> mHomeAssistApi = nullptr;
};

}  // namespace UI
