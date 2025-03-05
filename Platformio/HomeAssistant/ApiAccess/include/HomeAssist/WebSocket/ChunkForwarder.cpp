#include "HomeAssist/WebSocket/ChunkForwarder.hpp"

#include "HomeAssist/WebSocket/Api.hpp"
#include "HomeAssist/WebSocket/Session/ISession.hpp"

using namespace HAL::WebSocket::Json;

namespace HomeAssist::WebSocket {

ChunkForwarder::ChunkForwarder(Api& aApi) : mApi(aApi) {}

void ChunkForwarder::SetProcessor(std::shared_ptr<IChunkProcessor> aProcessor) {
  mProcessor = aProcessor;
}

bool ChunkForwarder::Null() {
  if (auto processor = mProcessor.lock()) {
    return processor->Null();
  }
  return false;
}
bool ChunkForwarder::Bool(bool b) {
  if (auto processor = mProcessor.lock()) {
    return processor->Bool(b);
  }
  return false;
}
bool ChunkForwarder::Int(int i) {
  if (mProcessingId) {
    mCurrentId = i;
    mFoundId = true;
    mProcessingId = false;

    if (auto& session = mApi.mSessions[mCurrentId]; session) {
      if (auto newProcessor = session->GetChunkProcessor(); newProcessor) {
        mProcessor = newProcessor;
      }
    }
  }

  if (auto processor = mProcessor.lock()) {
    return processor->Int(i);
  }
  return false;
}
bool ChunkForwarder::Uint(unsigned u) {
  if (auto processor = mProcessor.lock()) {
    return processor->Uint(u);
  }
  return false;
}
bool ChunkForwarder::Int64(int64_t i) {
  if (auto processor = mProcessor.lock()) {
    return processor->Int64(i);
  }
  return false;
}
bool ChunkForwarder::Uint64(uint64_t u) {
  if (auto processor = mProcessor.lock()) {
    return processor->Uint64(u);
  }
  return false;
}
bool ChunkForwarder::Double(double d) {
  if (auto processor = mProcessor.lock()) {
    return processor->Double(d);
  }
  return false;
}
bool ChunkForwarder::RawNumber(const Ch* str, rapidjson::SizeType length,
                               bool copy) {
  if (auto processor = mProcessor.lock()) {
    return processor->RawNumber(str, length, copy);
  }
  return false;
}
bool ChunkForwarder::String(const Ch* str, rapidjson::SizeType length,
                            bool copy) {
  if (auto processor = mProcessor.lock()) {
    return processor->String(str, length, copy);
  }
  return false;
}
bool ChunkForwarder::StartObject() {
  mInObject = true;
  if (auto processor = mProcessor.lock()) {
    return processor->StartObject();
  }
  return false;
}
bool ChunkForwarder::Key(const Ch* str, rapidjson::SizeType length, bool copy) {
  if (strcmp(str, "id") == 0) {
    mProcessingId = true;
  }
  if (auto processor = mProcessor.lock()) {
    return processor->Key(str, length, copy);
  }
  return false;
}
bool ChunkForwarder::EndObject(rapidjson::SizeType memberCount) {
  mInObject = false;
  mFoundId = false;
  mCurrentId = InvalidId;
  if (auto processor = mProcessor.lock()) {
    return processor->EndObject(memberCount);
  }
  return false;
}
bool ChunkForwarder::StartArray() {
  if (auto processor = mProcessor.lock()) {
    return processor->StartArray();
  }
  return false;
}
bool ChunkForwarder::EndArray(rapidjson::SizeType elementCount) {
  if (auto processor = mProcessor.lock()) {
    return processor->EndArray(elementCount);
  }
  return false;
}

}  // namespace HomeAssist::WebSocket
