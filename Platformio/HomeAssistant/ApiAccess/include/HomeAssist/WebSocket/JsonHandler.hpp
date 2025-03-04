#pragma once

#include "rapidjson/document.h"
#include "websockets/IProcessJsonMessage.hpp"

namespace HomeAssist::WebSocket {

class JsonHandler : public HAL::WebSocket::IProcessJsonMessage {
 public:
  JsonHandler(DocumentProccessor aProcessor);

  virtual ~JsonHandler();

  bool IsChunkProcessingPrefered() override;
};

}  // namespace HomeAssist::WebSocket