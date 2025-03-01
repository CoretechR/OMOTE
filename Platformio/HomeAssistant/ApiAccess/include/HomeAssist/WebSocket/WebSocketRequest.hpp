#pragma once

#include <string>

namespace HomeAssist::WebSocket {

class WebSocketRequest {
 public:
  WebSocketRequest(std::string aRequestMessage);
  ~WebSocketRequest() = default;

  std::string GetRequestMessage();

 private:
  std::string mRequestMessage;
};

inline WebSocketRequest::WebSocketRequest(std::string aRequestMessage)
    : mRequestMessage(aRequestMessage) {}

inline std::string WebSocketRequest::GetRequestMessage() {
  return mRequestMessage;
}

}  // namespace HomeAssist::WebSocket
