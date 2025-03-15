#pragma once
#include "IProcessMessage.hpp"
#include "rapidjson/reader.h"

namespace Json {

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

  /**
   * Callers:
   *   Update how far along we are in the processing of chunks
   * Impelemnters:
   *   Override to react to the amount of bytes we processes so far
   */
  virtual void UpdateProgress(size_t aProcessedBytes, size_t aTotalBytes) {};
  /**
   * Callers:
   *   Call on completion of parsing with result
   * Implementers:
   *   Override to handle when we finish processing
   */
  // Potential Improvement this tightly couples the Interfaces so we could make
  // a Process Result over here and then In IprocessMessage we could convert it
  // before we pass it in here.
  virtual void Completed(
      const IProcessMessage::ProcessResult& aCompletionResult) {};
};

}  // namespace Json
