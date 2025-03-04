#pragma once

#include <functional>

#include "RapidJsonUtilty.hpp"
#include "rapidjson/reader.h"

namespace HAL::WebSocket {

/**
 * This class is meant to help the websocket with json message handling
 */
class IHandleJsonMessage {
 public:
  using DocumentProccessor = std::function<bool(const MemConciousDocument&)>;

  IHandleJsonMessage() = default;
  virtual ~IHandleJsonMessage() = default;

  IHandleJsonMessage(DocumentProccessor aProccessor);
  /**
   * Return a reader that is prepared to handle a large message chunk by chunk
   */
  virtual rapidjson::BaseReaderHandler<>* BorrowLargeMessageHander() = 0;

  /**
   * Return true if processed successfully false otherwise
   */
  bool ProcessDocument(const MemConciousDocument& aRecievedDocument);

 private:
  DocumentProccessor mProcessor = nullptr;
};

inline IHandleJsonMessage::IHandleJsonMessage(DocumentProccessor aProcessor)
    : mProcessor(aProcessor) {}

inline bool IHandleJsonMessage::ProcessDocument(
    const MemConciousDocument& aRecievedDocument) {
  if (mProcessor) {
    return mProcessor(aRecievedDocument);
  }
  return false;
}

}  // namespace HAL::WebSocket