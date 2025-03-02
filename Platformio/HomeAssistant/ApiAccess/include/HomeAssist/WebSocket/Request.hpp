#pragma once

#include <string>

#include "RapidJsonUtilty.hpp"
#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

class Request {
 public:
  Request(rapidjson::Document aRequestMessage);
  Request(std::string aRequestMessage);
  ~Request() = default;

  std::string GetRequestMessage();

  void SetId(int aId);

 private:
  rapidjson::Document mRequestMessage;
};

inline Request::Request(std::string aRequestMessage) {
  mRequestMessage.Parse(aRequestMessage.c_str());
}

inline std::string Request::GetRequestMessage() {
  return ToString(mRequestMessage);
}

inline void Request::SetId(int aId) { mRequestMessage["id"] = aId; }

}  // namespace HomeAssist::WebSocket
