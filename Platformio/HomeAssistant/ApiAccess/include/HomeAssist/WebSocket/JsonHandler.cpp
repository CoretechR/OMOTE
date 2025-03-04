#include "HomeAssist/WebSocket/JsonHandler.hpp"

using namespace HAL::WebSocket;

namespace HomeAssist::WebSocket {

JsonHandler::JsonHandler(IHandleJsonMessage::DocumentProccessor aProcessor)
    : IHandleJsonMessage(aProcessor) {}

JsonHandler::~JsonHandler() {}

rapidjson::BaseReaderHandler<>* JsonHandler::BorrowLargeMessageHander() {
  // Implement your handler here
  return nullptr;
}

bool JsonHandler::IsLargeHandlerPrefered() {
  // Implement your logic here
  return false;
}

}  // namespace HomeAssist::WebSocket
