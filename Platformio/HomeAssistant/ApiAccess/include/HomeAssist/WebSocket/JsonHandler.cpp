#include "HomeAssist/WebSocket/JsonHandler.hpp"

using namespace HAL::WebSocket;

namespace HomeAssist::WebSocket {

JsonHandler::JsonHandler(IHandleJsonMessage::DocumentProccessor aProcessor)
    : IHandleJsonMessage(aProcessor) {}

JsonHandler::~JsonHandler() {}

bool JsonHandler::IsChunkProcessingPrefered() {
  // Implement your logic here
  return false;
}

}  // namespace HomeAssist::WebSocket
