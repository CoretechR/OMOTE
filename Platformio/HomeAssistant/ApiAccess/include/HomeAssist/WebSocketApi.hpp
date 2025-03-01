#pragma once

#include <memory>

#include "webSocketInterface.hpp"

namespace HomeAssist {

class WebSocketApi {
 public:
  WebSocketApi(std::shared_ptr<webSocketInterface> socket);
  virtual ~WebSocketApi();

 private:
  std::shared_ptr<webSocketInterface> mHomeAssistSocket = nullptr;
};

}  // namespace HomeAssist
