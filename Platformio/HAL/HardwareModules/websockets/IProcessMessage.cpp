#include "IProcessMessage.hpp"

namespace HAL::WebSocket::Json {

IProcessMessage::ProcessResult::ProcessResult(ProcessResult::StatusCode aStatus,
                                              rapidjson::ParseResult aResult)
    : mStatus(aStatus), mParseResult(aResult) {}

IProcessMessage::ProcessResult::ProcessResult(rapidjson::ParseResult aResult)
    : mParseResult(aResult) {
  mStatus = mParseResult.IsError() ? ProcessResult::StatusCode::ParseError
                                   : ProcessResult::StatusCode::Success;
}

IProcessMessage::ProcessResult::ProcessResult(rapidjson::ParseErrorCode aError)
    : mStatus(StatusCode::ParseError), mParseResult(aError, 0) {}

IProcessMessage::ProcessResult::operator bool() {
  return mStatus == ProcessResult::StatusCode::Success;
}

IProcessMessage::IProcessMessage(
    DocumentProccessor aDocProcessor,
    std::unique_ptr<IChunkProcessor> aChunkProcessor)
    : mDocProcessor(aDocProcessor),
      mChunkProcessor(std::move(aChunkProcessor)) {}

bool IProcessMessage::ProcessDocument(
    const MemConciousDocument& aRecievedDocument) {
  if (mDocProcessor) {
    return mDocProcessor(aRecievedDocument);
  }
  return false;
}

bool IProcessMessage::HasChunkProcessor() { return mChunkProcessor != nullptr; }

IProcessMessage::ProcessResult IProcessMessage::ProcessChunk(
    const std::string& aJsonChunk) {
  if (!mChunkProcessor) {
    return {ProcessResult::StatusCode::MissingChunkProcessor};
  }
  auto stream = rapidjson::StringStream(aJsonChunk.c_str());
  return {mChunkReader.Parse(stream, *mChunkProcessor)};
}

IProcessMessage::ProcessResult IProcessMessage::ProcessJsonAsDoc(
    const std::string& aJsonString) {
  MemConciousDocument aDoc;
  // Warning this should be safe since I only use this in ProcessDocument
  // which does not allow downstream code to change the Doc since it is const
  aDoc.ParseInsitu(const_cast<char*>(aJsonString.data()));
  if (aDoc.HasParseError()) {
    return {aDoc.GetParseError()};
  }
  if (!ProcessDocument(aDoc)) {
    return {ProcessResult::StatusCode::DocProcessorFailed};
  }
  return {ProcessResult::StatusCode::Success};
}

bool IProcessMessage::IsChunkProcessingPrefered() {
  // Only processor we have is chunk probably use it.
  return mChunkProcessor && !mDocProcessor;
}

}  // namespace HAL::WebSocket::Json
