#pragma once

#include <functional>

#include "IChunkProcessor.hpp"
#include "RapidJsonUtilty.hpp"
#include "rapidjson/error/error.h"
#include "rapidjson/reader.h"
#include "rapidjson/stream.h"

namespace HAL::WebSocket::Json {

/**
 * This class is meant to help the websocket with json message handling
 */
class IProcessMessage {
 public:
  struct ProcessResult {
    enum class StatusCode {
      Success,
      SuccessFinishedChunkParse,
      SuccessWaitingForNextChunk,
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

  IProcessMessage() = default;
  IProcessMessage(DocumentProccessor aProccessor = nullptr,
                  std::unique_ptr<IChunkProcessor> aChunkProcessor = nullptr);
  virtual ~IProcessMessage() = default;

  // Document Based Processing
  ProcessResult ProcessJsonAsDoc(const std::string& aJsonString);

  // Chunk Based Processing
  bool HasChunkProcessor();
  ProcessResult ProcessChunk(const std::string& aJsonChunk);

  virtual bool IsChunkProcessingPrefered();

 protected:
  bool ProcessDocument(const MemConciousDocument& aRecievedDocument);

 private:
  DocumentProccessor mDocProcessor = nullptr;

  rapidjson::Reader mChunkReader;
  rapidjson::Reader mSucessfulChunkReader;
  std::unique_ptr<IChunkProcessor> mChunkProcessor = nullptr;
  bool mIsProcessingChunks = false;
  std::string mUnprocessedStr;
};

}  // namespace HAL::WebSocket::Json
