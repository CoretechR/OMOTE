#pragma once

#include <string>

#include "RapidJsonUtilty.hpp"
#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

class Request {
 public:
  Request(std::string aRequestMessage);
  ~Request() = default;

  std::string GetRequestMessage();

  void SetId(int aId);

 private:
  MemConciousDocument mRequestMessage;
};

inline Request::Request(std::string aRequestMessage)
    : mRequestMessage(GetDocument(aRequestMessage)) {}

inline std::string Request::GetRequestMessage() {
  return ToString(mRequestMessage);
}

inline void Request::SetId(int aId) { mRequestMessage["id"] = aId; }

}  // namespace HomeAssist::WebSocket
