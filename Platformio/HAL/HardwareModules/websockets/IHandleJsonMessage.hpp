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
class IHandleJsonMessage {
 public:
  struct ProcessResult {
    enum class ErrorCode { Success, ParseError, MissingChunkProcessor };
    ProcessResult(rapidjson::ParseResult aResult, ErrorCode aInternalError);

    ErrorCode mError;
    rapidjson::ParseResult mParseResult;
  };

  using DocumentProccessor = std::function<bool(const MemConciousDocument&)>;
  using ChunkProcessor = rapidjson::BaseReaderHandler<>;

  IHandleJsonMessage() = default;
  IHandleJsonMessage(DocumentProccessor aProccessor = nullptr,
                     std::unique_ptr<ChunkProcessor> aChunkProcessor = nullptr);
  virtual ~IHandleJsonMessage() = default;

  // Document Based Processing
  bool ProcessDocument(const MemConciousDocument& aRecievedDocument);
  rapidjson::ParseResult ProcessJsonAsDoc(std::string& aJsonString);

  // Chunk Based Processing
  bool HasChunkProcessor();
  rapidjson::ParseResult ProcessChunk(const std::string& aJsonChunk);

  virtual bool IsChunkProcessingPrefered() = 0;

 private:
  DocumentProccessor mDocProcessor = nullptr;

  rapidjson::Reader mChunkReader;
  std::unique_ptr<ChunkProcessor> mChunkProcessor = nullptr;
};

inline IHandleJsonMessage::IHandleJsonMessage(
    DocumentProccessor aDocProcessor,
    std::unique_ptr<ChunkProcessor> aChunkProcessor)
    : mDocProcessor(aDocProcessor),
      mChunkProcessor(std::move(aChunkProcessor)) {}

inline bool IHandleJsonMessage::ProcessDocument(
    const MemConciousDocument& aRecievedDocument) {
  if (mDocProcessor) {
    return mDocProcessor(aRecievedDocument);
  }
  return false;
}

inline bool IHandleJsonMessage::HasChunkProcessor() {
  return mChunkProcessor != nullptr;
}

inline rapidjson::ParseResult IHandleJsonMessage::ProcessChunk(
    const std::string& aJsonChunk) {
  if (!mChunkProcessor) {
    return rapidjson::ParseResult(
        rapidjson::ParseErrorCode::kParseErrorUnspecificSyntaxError, 0);
  }
  auto stream = rapidjson::StringStream(aJsonChunk.c_str());
  return mChunkReader.Parse(stream, *mChunkProcessor);
}

inline rapidjson::ParseResult IHandleJsonMessage::ProcessJsonAsDoc(
    std::string& aJsonString) {
  MemConciousDocument aDoc;
  return aDoc.ParseInsitu(aJsonString.data());
  // mDocProcessor(aDoc);
}

inline IHandleJsonMessage::ProcessResult::ProcessResult(
    rapidjson::ParseResult aResult, ErrorCode aInternalError)
    : mParseResult(aResult), mError(aInternalError) {}

}  // namespace HAL::WebSocket