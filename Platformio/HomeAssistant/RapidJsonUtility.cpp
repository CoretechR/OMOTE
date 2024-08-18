#include "RapidJsonUtilty.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

std::string ToString(rapidjson::Document &aDoc) {
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
  aDoc.Accept(writer);
  return std::string(buff.GetString());
}