#include "RapidJsonUtilty.hpp"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

std::string ToString(rapidjson::Document &aDoc) {
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

const rapidjson::Value *GetNestedField(
    const rapidjson::Value &aValue, const std::vector<std::string> &aFields) {
  const rapidjson::Value *value = &aValue;
  for (const auto &field : aFields) {
    if (!value || !value->IsObject() || !value->HasMember(field.c_str())) {
      return nullptr;
    }
    value = &(*value)[field.c_str()];
  }
  return value;
}