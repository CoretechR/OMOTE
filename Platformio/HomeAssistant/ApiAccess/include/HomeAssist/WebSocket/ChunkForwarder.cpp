#include "HomeAssist/WebSocket/ChunkForwarder.hpp"

namespace HomeAssist::WebSocket {

bool ChunkForwarder::Null() { return false; }
bool ChunkForwarder::Bool(bool b) { return false; }
bool ChunkForwarder::Int(int i) { return false; }
bool ChunkForwarder::Uint(unsigned u) { return false; }
bool ChunkForwarder::Int64(int64_t i) { return false; }
bool ChunkForwarder::Uint64(uint64_t u) { return false; }
bool ChunkForwarder::Double(double d) { return false; }
bool ChunkForwarder::RawNumber(const Ch* str, rapidjson::SizeType length,
                               bool copy) {
  return false;
}
bool ChunkForwarder::String(const Ch* str, rapidjson::SizeType length,
                            bool copy) {
  return false;
}
bool ChunkForwarder::StartObject() { return false; }
bool ChunkForwarder::Key(const Ch* str, rapidjson::SizeType length, bool copy) {
  return false;
}
bool ChunkForwarder::EndObject(rapidjson::SizeType memberCount) {
  return false;
}
bool ChunkForwarder::StartArray() { return false; }
bool ChunkForwarder::EndArray(rapidjson::SizeType elementCount) {
  return false;
}

}  // namespace HomeAssist::WebSocket
