#pragma once
#include "BasicUI.hpp"
#include "HomeAssist/WebSocket/Api.hpp"

namespace UI {

class HomeAssistUI : public BasicUI {
 public:
  HomeAssistUI();

 private:
  std::unique_ptr<HomeAssist::WebSocket::Api> mHomeAssistSock = nullptr;
};

}  // namespace UI
