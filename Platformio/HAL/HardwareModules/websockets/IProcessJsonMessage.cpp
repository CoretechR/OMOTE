#include "IProcessJsonMessage.hpp"

namespace HAL::WebSocket {

IProcessJsonMessage::ProcessResult::ProcessResult(
    ProcessResult::StatusCode aStatus, rapidjson::ParseResult aResult)
    : mStatus(aStatus), mParseResult(aResult) {}

IProcessJsonMessage::ProcessResult::ProcessResult(
    rapidjson::ParseResult aResult)
    : mParseResult(aResult) {
  mStatus = mParseResult.IsError() ? ProcessResult::StatusCode::ParseError
                                   : ProcessResult::StatusCode::Success;
}

IProcessJsonMessage::ProcessResult::ProcessResult(
    rapidjson::ParseErrorCode aError)
    : mStatus(StatusCode::ParseError), mParseResult(aError, 0) {}

IProcessJsonMessage::ProcessResult::operator bool() {
  return mStatus == ProcessResult::StatusCode::Success;
}

IProcessJsonMessage::IProcessJsonMessage(
    DocumentProccessor aDocProcessor,
    std::unique_ptr<ChunkProcessor> aChunkProcessor)
    : mDocProcessor(aDocProcessor),
      mChunkProcessor(std::move(aChunkProcessor)) {}

bool IProcessJsonMessage::ProcessDocument(
    const MemConciousDocument& aRecievedDocument) {
  if (mDocProcessor) {
    return mDocProcessor(aRecievedDocument);
  }
  return false;
}

bool IProcessJsonMessage::HasChunkProcessor() {
  return mChunkProcessor != nullptr;
}

IProcessJsonMessage::ProcessResult IProcessJsonMessage::ProcessChunk(
    const std::string& aJsonChunk) {
  if (!mChunkProcessor) {
    return {ProcessResult::StatusCode::MissingChunkProcessor};
  }
  auto stream = rapidjson::StringStream(aJsonChunk.c_str());
  return {mChunkReader.Parse(stream, *mChunkProcessor)};
}

IProcessJsonMessage::ProcessResult IProcessJsonMessage::ProcessJsonAsDoc(
    std::string& aJsonString) {
  MemConciousDocument aDoc;
  aDoc.ParseInsitu(aJsonString.data());
  if (aDoc.HasParseError()) {
    return {aDoc.GetParseError()};
  }
  if (!ProcessDocument(aDoc)) {
    return {ProcessResult::StatusCode::DocProcessorFailed};
  }
  return {ProcessResult::StatusCode::Success};
}

bool IProcessJsonMessage::IsChunkProcessingPrefered() {
  // Only processor we have is chunk probably use it.
  return mChunkProcessor && !mDocProcessor;
}

}  // namespace HAL::WebSocket
