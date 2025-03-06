#include "IProcessMessage.hpp"

#include "IChunkProcessor.hpp"

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

IProcessMessage::IProcessMessage() = default;

IProcessMessage::IProcessMessage(
    DocumentProccessor aDocProcessor,
    std::unique_ptr<IChunkProcessor> aChunkProcessor)
    : mDocProcessor(aDocProcessor),
      mChunkProcessor(std::move(aChunkProcessor)),
      mUnprocessedBuffer(DefaultMaxBufferSize, '\0') {}

IProcessMessage::~IProcessMessage() = default;

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
    const std::string& aJsonChunk, const size_t aTotalJsonSize) {
  if (!mChunkProcessor) {
    return {ProcessResult::StatusCode::MissingChunkProcessor};
  }
  if (!IsProcessingChunks()) {
    mChunkReader.IterativeParseInit();
    mCurrentChunkBasedTotalJsonSize = aTotalJsonSize;
  } else {
    if (aTotalJsonSize != mCurrentChunkBasedTotalJsonSize) {
      return {
          ProcessResult::StatusCode::FailedChunkProcessingMessageSizeChanged};
    }
  }

  if (mUnprocessedBuffer.length() + aJsonChunk.length() > mMaxBufferSize) {
    return {ProcessResult::StatusCode::FailedChunkProcessorMemoryLimitMet};
  }

  mUnprocessedBuffer += aJsonChunk;

  auto stream = rapidjson::StringStream(mUnprocessedBuffer.c_str());
  auto lastSuccessfulReadIndex = 0;

  while (mChunkReader.IterativeParseNext<rapidjson::kParseIterativeFlag>(
      stream, *mChunkProcessor)) {
    if (mChunkReader.IterativeParseComplete()) {
      mCurrentChunkBasedTotalJsonSize = mCurrentChunkBasedTotalJsonSize;
      ProcessResult result = {
          ProcessResult::StatusCode::SuccessFinishedChunkParse};
      EndChunkProcessing(result);
      return result;
    }
    if (!mChunkReader.HasParseError()) {
      lastSuccessfulReadIndex = stream.Tell();
      if (IsChunkBufferToSmallForProcessing(lastSuccessfulReadIndex)) {
        mUnprocessedBuffer = mUnprocessedBuffer.substr(lastSuccessfulReadIndex);
        mChunkProcessor->UpdateProgress(mOffsetIntoChunkBasedJson,
                                        mCurrentChunkBasedTotalJsonSize);
        return {ProcessResult::StatusCode::SuccessWaitingForNextChunk};
      }
    }
  }

  ProcessResult result = {
      ProcessResult::StatusCode::ParseError,
      {mChunkReader.GetParseErrorCode(), mOffsetIntoChunkBasedJson}};
  EndChunkProcessing(result);
  return result;
}

bool IProcessMessage::IsProcessingChunks() const {
  mCurrentChunkBasedTotalJsonSize != ChunkMessageSizeNotProcessing;
}
void IProcessMessage::EndChunkProcessing(
    const ProcessResult& aResultToEndWith) {
  mCurrentChunkBasedTotalJsonSize = ChunkMessageSizeNotProcessing;
  mOffsetIntoChunkBasedJson = 0;
  if (mChunkProcessor) {
    mChunkProcessor->Completed(aResultToEndWith);
  }
  // Clear the buffer
  mUnprocessedBuffer.clear();
  mUnprocessedBuffer.shrink_to_fit();
}

bool IProcessMessage::IsChunkBufferToSmallForProcessing(
    size_t aCurrentIndexIntoBuffer) const {
  // TODO: Consider this lookAheadsize abit more
  const auto lookAheadBytes = mMaxBufferSize * .5;
  return mUnprocessedBuffer.length() - aCurrentIndexIntoBuffer < lookAheadBytes;
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

void IProcessMessage::SetMaxProcessBufferSize(size_t aProcessBufferSize) {
  mMaxBufferSize = aProcessBufferSize;
  mUnprocessedBuffer.reserve(mMaxBufferSize);
}

}  // namespace HAL::WebSocket::Json
