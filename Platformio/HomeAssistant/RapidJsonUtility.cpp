#include "HomeAssistant/RapidJsonUtilty.hpp"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

void MemConciousAllocator::Free(void *aVal) {}

void BuffDeleter::operator()(void *buffer) { free(buffer); }

void *MemConciousAllocator::Malloc(size_t aSize) {
  mBuffers.emplace_back(aSize);
  return mBuffers.back().data();
}
void *MemConciousAllocator::Realloc(void *originalPtr, size_t originalSize,
                                    size_t newSize) {
  if (originalPtr == nullptr) {
    return Malloc(newSize);
  }
  for (auto &buffer : mBuffers) {
    if (originalPtr == buffer.data()) {
      buffer.resize(newSize);
      return buffer.data();
    }
  }
  // Told us to realloc but didn't know about old buffer... bad
  return nullptr;
}

std::string ToString(const rapidjson::Document &aDoc) {
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
  aDoc.Accept(writer);
  return std::string(buff.GetString());
}

std::string ToPrettyString(rapidjson::Document &aDoc) {
  rapidjson::StringBuffer buff;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWrite(buff);
  prettyWrite.SetIndent(' ', 2);
  aDoc.Accept(prettyWrite);
  return std::string(buff.GetString());
}

const MemConciousValue *GetNestedField(
    const MemConciousValue &aValue, const std::vector<std::string> &aFields) {
  const MemConciousValue *value = &aValue;
  for (const auto &field : aFields) {
    if (!value || !value->IsObject() || !value->HasMember(field.c_str())) {
      return nullptr;
    }
    value = &(*value)[field.c_str()];
  }
  return value;
}

rapidjson::GenericDocument<rapidjson::UTF8<>, MemConciousAllocator> GetDocument(
    const std::string &aStringToParse) {
  rapidjson::GenericDocument<rapidjson::UTF8<>, MemConciousAllocator> doc;
  doc.Parse(aStringToParse.c_str());
  return doc;
}

std::string ToString(const MemConciousDocument &aDoc) {
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
  aDoc.Accept(writer);
  return std::string(buff.GetString());
}