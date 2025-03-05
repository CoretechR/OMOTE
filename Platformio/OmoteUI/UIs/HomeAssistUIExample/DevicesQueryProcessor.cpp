#include "DevicesQueryProcessor.hpp"

namespace UI {

bool DevicesQueryProcessor::Null() { return false; }
bool DevicesQueryProcessor::Bool(bool b) { return false; }
bool DevicesQueryProcessor::Int(int i) { return false; }
bool DevicesQueryProcessor::Uint(unsigned u) { return false; }
bool DevicesQueryProcessor::Int64(int64_t i) { return false; }
bool DevicesQueryProcessor::Uint64(uint64_t u) { return false; }
bool DevicesQueryProcessor::Double(double d) { return false; }
bool DevicesQueryProcessor::RawNumber(const Ch* str, rapidjson::SizeType length,
                                      bool copy) {
  return false;
}
bool DevicesQueryProcessor::String(const Ch* str, rapidjson::SizeType length,
                                   bool copy) {
  return false;
}
bool DevicesQueryProcessor::StartObject() { return false; }
bool DevicesQueryProcessor::Key(const Ch* str, rapidjson::SizeType length,
                                bool copy) {
  return false;
}
bool DevicesQueryProcessor::EndObject(rapidjson::SizeType memberCount) {
  return false;
}
bool DevicesQueryProcessor::StartArray() { return false; }
bool DevicesQueryProcessor::EndArray(rapidjson::SizeType elementCount) {
  return false;
}

}  // namespace UI
