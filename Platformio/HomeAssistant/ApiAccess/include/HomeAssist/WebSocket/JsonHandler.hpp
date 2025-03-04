#pragma once

#include "rapidjson/document.h"
#include "websockets/IHandleJsonMessage.hpp"

namespace HomeAssist::WebSocket {

class JsonHandler : public HAL::WebSocket::IHandleJsonMessage {
 public:
  JsonHandler(DocumentProccessor aProcessor);

  virtual ~JsonHandler();

  rapidjson::BaseReaderHandler<>* BorrowLargeMessageHander() override;

  bool IsLargeHandlerPrefered() override;
};

}  // namespace HomeAssist::WebSocket