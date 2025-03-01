#pragma once
#include "BasicUI.hpp"
#include "HomeAssist/WebSocketApi.hpp"

namespace UI {

class HomeAssistUI : public BasicUI {
 public:
  HomeAssistUI();

 private:
  std::unique_ptr<HomeAssist::WebSocketApi> mHomeAssistSock = nullptr;
};

}  // namespace UI
