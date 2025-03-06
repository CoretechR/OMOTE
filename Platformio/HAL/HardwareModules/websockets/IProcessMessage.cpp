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
  using Result = ProcessResult::StatusCode;
  return mStatus == Result::Success ||
         mStatus == Result::SuccessFinishedChunkParse ||
         mStatus == Result::SuccessWaitingForNextChunk;
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

// MatthewColvin/OMOTE#6 pass final size in here and track currently processed
// byte to allow for for calling status update callback
IProcessMessage::ProcessResult IProcessMessage::ProcessChunk(
    const std::string& aJsonChunk) {
  if (!mChunkProcessor) {
    return {ProcessResult::StatusCode::MissingChunkProcessor};
  }
  if (!mIsProcessingChunks) {
    mChunkReader.IterativeParseInit();
    mIsProcessingChunks = true;
  }

  // MatthewColvin/OMOTE#6 todo update to honor max size and if we cant handle
  // it bail with the FailedChunkProcessorMemoryLimitMet error
  auto chunkToProcess =
      mUnprocessedStr.empty() ? aJsonChunk : mUnprocessedStr + aJsonChunk;

  auto stream = rapidjson::StringStream(chunkToProcess.c_str());
  auto lastSuccessfulReadIndex = 0;
  // Todo
  // Honeslty not sure if this can ever be completely valid
  // Probably just need a better look ahead method
  static constexpr auto MagicLookAheadedBufferLength = 300;
  while (mChunkReader.IterativeParseNext<rapidjson::kParseIterativeFlag>(
      stream, *mChunkProcessor)) {
    if (!mChunkReader.HasParseError()) {
      lastSuccessfulReadIndex = stream.Tell();
      auto isNotMuchTextLeft =
          chunkToProcess.length() - lastSuccessfulReadIndex <
          MagicLookAheadedBufferLength;
      if (isNotMuchTextLeft) {
        mUnprocessedStr = chunkToProcess.substr(lastSuccessfulReadIndex);
        return {ProcessResult::StatusCode::SuccessWaitingForNextChunk};
        // MatthewColvin/OMOTE#6 call percent complete callback here
      }
    }
    if (mChunkReader.IterativeParseComplete()) {
      mIsProcessingChunks = false;
      return {ProcessResult::StatusCode::SuccessFinishedChunkParse};
      // MatthewColvin/OMOTE#6 this could be where we call a "chunk parse
      // complete callback with the ProcessResult
    }
  }
  // TODO what really?
  return {ProcessResult::StatusCode::SuccessWaitingForNextChunk};
}

IProcessMessage::ProcessResult IProcessMessage::ProcessJsonAsDoc(
    const std::string& aJsonString) {
  MemConciousDocument aDoc;
  aDoc.Parse(aJsonString.data());
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
