#pragma once

#include "HomeAssist/WebSocket/Api.hpp"
#include "rapidjson/document.h"
#include "websockets/IProcessJsonMessage.hpp"

namespace HomeAssist::WebSocket {

class ResponseHandler : public HAL::WebSocket::Json::IProcessJsonMessage {
 public:
  ResponseHandler(HomeAssist::WebSocket::Api& aApi);
  virtual ~ResponseHandler();

  bool ProcessResponseDoc(const MemConciousDocument& aDoc);

 private:
  HomeAssist::WebSocket::Api& mApi;
};

}  // namespace HomeAssist::WebSocket