#pragma once

#include "HomeAssistant/Api/WebSocket/Api.hpp"
#include "rapidjson/document.h"
#include "websockets/IProcessMessage.hpp"

namespace HomeAssist::WebSocket {

class ResponseHandler : public HAL::WebSocket::Json::IProcessMessage {
 public:
  ResponseHandler(HomeAssist::WebSocket::Api& aApi);
  virtual ~ResponseHandler();

  bool ProcessResponseDoc(const MemConciousDocument& aDoc);

  /**
   * @return true - chunk processor in a session handled the doc
   *         fasle - session didn't want the doc processed that way
   */
  bool HandleRedirectToChunkProcessor(const MemConciousDocument& aDoc);

 private:
  HomeAssist::WebSocket::Api& mApi;
};

}  // namespace HomeAssist::WebSocket