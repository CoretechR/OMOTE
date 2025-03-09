#pragma once

#include <functional>

#include "RapidJsonUtilty.hpp"
#include "rapidjson/error/error.h"
#include "rapidjson/reader.h"
#include "rapidjson/stream.h"

namespace HAL::WebSocket::Json {
class IChunkProcessor;

/**
 * This class is meant to help the websocket with json message handling
 * It does this in 2 ways.
 *
 * 1) Document based processing for easier handling in downstream code for
 * small strings
 *
 * 2) Dynamically sized ring buffer processing allowing the user
 * of this interface to apply an upperlimit on the amount of memory they want it
 * to use for the parse **not including rapidjsons usage**. This utalizes the
 * RapidJson SAX parser with a virtual twist to allow dependency injection from
 * higher level along with a feedback mechanism for progress see
 * IChunkProcessor interface.
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
      FailedChunkProcessorMemoryLimitMet,
      FailedChunkProcessingMessageSizeChanged,  // Can only process one message
                                                // at a time so if size changed
                                                // were in bad state
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

  IProcessMessage();
  IProcessMessage(DocumentProccessor aProccessor = nullptr,
                  std::unique_ptr<IChunkProcessor> aChunkProcessor = nullptr);
  virtual ~IProcessMessage();

  // Document Based Processing
  ProcessResult ProcessJsonAsDoc(const std::string& aJsonString);

  // Chunk Based Processing
  bool HasChunkProcessor();
  ProcessResult ProcessChunk(const std::string& aJsonChunk,
                             size_t aTotalJsonSize);

  virtual bool IsChunkProcessingPrefered();

  void SetMaxProcessBufferSize(size_t aProcessBufferSize);

 protected:
  bool ProcessDocument(const MemConciousDocument& aRecievedDocument);

 private:
  DocumentProccessor mDocProcessor = nullptr;
  bool IsProcessingChunks() const;
  bool IsChunkBufferToSmallForProcessing() const;

  void UpdateBufferAndMetaData();
  void EndChunkProcessing(const ProcessResult& aResultToEndWith);

  rapidjson::Reader mChunkReader;
  rapidjson::StringStream mChunkStream;

  std::unique_ptr<IChunkProcessor> mChunkProcessor;
  static constexpr auto ChunkMessageSizeNotProcessing = 0;
  size_t mCurrentChunkBasedTotalJsonSize = ChunkMessageSizeNotProcessing;
  size_t mOffsetIntoChunkBasedJson = 0;
  std::string mUnprocessedBuffer;

  static constexpr auto DefaultMaxBufferSize = 1024;
  size_t mMaxBufferSize = DefaultMaxBufferSize;
};

}  // namespace HAL::WebSocket::Json
