#pragma once
#include "rapidjson/reader.h"

namespace HAL::WebSocket::Json {

class IChunkProcessor
    : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, IChunkProcessor> {
 public:
  virtual ~IChunkProcessor() = default;

  virtual bool Null() = 0;
  virtual bool Bool(bool b) = 0;
  virtual bool Int(int i) = 0;
  virtual bool Uint(unsigned u) = 0;
  virtual bool Int64(int64_t i) = 0;
  virtual bool Uint64(uint64_t u) = 0;
  virtual bool Double(double d) = 0;
  virtual bool RawNumber(const Ch* str, rapidjson::SizeType length,
                         bool copy) = 0;
  virtual bool String(const Ch* str, rapidjson::SizeType length, bool copy) = 0;
  virtual bool StartObject() = 0;
  virtual bool Key(const Ch* str, rapidjson::SizeType length, bool copy) = 0;
  virtual bool EndObject(rapidjson::SizeType memberCount) = 0;
  virtual bool StartArray() = 0;
  virtual bool EndArray(rapidjson::SizeType elementCount) = 0;
};

}  // namespace HAL::WebSocket::Json
