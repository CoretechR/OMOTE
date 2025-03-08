#pragma once

#include <string>

#include "HomeAssistant/RapidJsonUtilty.hpp"
#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

class Request {
 public:
  Request(MemConciousDocument&& aRequestMessage);  // Changed constructor
  ~Request() = default;

  std::string GetRequestMessage();

  void SetId(int aId);

 private:
  MemConciousDocument mRequestMessage;
};

inline Request::Request(MemConciousDocument&& aRequestMessage)
    : mRequestMessage(std::move(aRequestMessage)) {}

inline std::string Request::GetRequestMessage() {
  return ToString(mRequestMessage);
}

inline void Request::SetId(int aId) {
  auto& alloc = mRequestMessage.GetAllocator();
  if (!mRequestMessage.HasMember("id")) {
    mRequestMessage.AddMember("id", MemConciousValue().SetInt(aId), alloc);
  }
  mRequestMessage["id"] = aId;
}

}  // namespace HomeAssist::WebSocket
