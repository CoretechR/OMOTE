#pragma once

#include <string>

namespace HomeAssist::WebSocket {

class Request {
 public:
  Request(std::string aRequestMessage);
  ~Request() = default;

  std::string GetRequestMessage();

 private:
  std::string mRequestMessage;
};

inline Request::Request(std::string aRequestMessage)
    : mRequestMessage(aRequestMessage) {}

inline std::string Request::GetRequestMessage() { return mRequestMessage; }

}  // namespace HomeAssist::WebSocket
