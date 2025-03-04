#include "IProcessJsonMessage.hpp"

namespace HAL::WebSocket {

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

rapidjson::ParseResult IProcessJsonMessage::ProcessChunk(
    const std::string& aJsonChunk) {
  if (!mChunkProcessor) {
    return rapidjson::ParseResult(
        rapidjson::ParseErrorCode::kParseErrorUnspecificSyntaxError, 0);
  }
  auto stream = rapidjson::StringStream(aJsonChunk.c_str());
  return mChunkReader.Parse(stream, *mChunkProcessor);
}

rapidjson::ParseResult IProcessJsonMessage::ProcessJsonAsDoc(
    std::string& aJsonString) {
  MemConciousDocument aDoc;
  return aDoc.ParseInsitu(aJsonString.data());
}

IProcessJsonMessage::ProcessResult::ProcessResult(
    rapidjson::ParseResult aResult, ErrorCode aInternalError)
    : mParseResult(aResult), mError(aInternalError) {}

}  // namespace HAL::WebSocket
