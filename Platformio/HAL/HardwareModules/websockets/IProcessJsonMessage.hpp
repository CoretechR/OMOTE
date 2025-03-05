#pragma once

#include <functional>

#include "RapidJsonUtilty.hpp"
#include "rapidjson/error/error.h"
#include "rapidjson/reader.h"
#include "rapidjson/stream.h"

namespace HAL::WebSocket {

/**
 * This class is meant to help the websocket with json message handling
 */
class IProcessJsonMessage {
 public:
  struct ProcessResult {
    enum class StatusCode {
      Success,
      ParseError,
      MissingChunkProcessor,
      DocProcessorFailed
    };
    ProcessResult(StatusCode aInternalError,
                  rapidjson::ParseResult aResult = {});
    ProcessResult(rapidjson::ParseResult aResult);
    ProcessResult(rapidjson::ParseErrorCode aError);

    operator bool();

    StatusCode mStatus{};
    rapidjson::ParseResult mParseResult{};
  };

  using DocumentProccessor = std::function<bool(const MemConciousDocument&)>;
  using ChunkProcessor = rapidjson::BaseReaderHandler<>;

  IProcessJsonMessage() = default;
  IProcessJsonMessage(
      DocumentProccessor aProccessor = nullptr,
      std::unique_ptr<ChunkProcessor> aChunkProcessor = nullptr);
  virtual ~IProcessJsonMessage() = default;

  // Document Based Processing
  ProcessResult ProcessJsonAsDoc(std::string& aJsonString);

  // Chunk Based Processing
  bool HasChunkProcessor();
  ProcessResult ProcessChunk(const std::string& aJsonChunk);

  virtual bool IsChunkProcessingPrefered();

 protected:
  bool ProcessDocument(const MemConciousDocument& aRecievedDocument);

 private:
  DocumentProccessor mDocProcessor = nullptr;

  rapidjson::Reader mChunkReader;
  std::unique_ptr<ChunkProcessor> mChunkProcessor = nullptr;
};

}  // namespace HAL::WebSocket
