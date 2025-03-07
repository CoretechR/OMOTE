#pragma once
#include <functional>
#include <string>

#include "websockets/IChunkProcessor.hpp"

namespace UI {

class DevicesQueryProcessor : public HAL::WebSocket::Json::IChunkProcessor {
 public:
  using resultType = HAL::WebSocket::Json::IProcessMessage::ProcessResult;

  using EntityIdCallback = std::function<void(const std::string&)>;

  explicit DevicesQueryProcessor(EntityIdCallback onEntityFound)
      : entityIdCallback(std::move(onEntityFound)) {}
  virtual ~DevicesQueryProcessor() = default;

  bool Null() override;
  bool Bool(bool b) override;
  bool Int(int i) override;
  bool Uint(unsigned u) override;
  bool Int64(int64_t i) override;
  bool Uint64(uint64_t u) override;
  bool Double(double d) override;
  bool RawNumber(const Ch* str, rapidjson::SizeType length, bool copy) override;
  bool String(const Ch* str, rapidjson::SizeType length, bool copy) override;
  bool StartObject() override;
  bool Key(const Ch* str, rapidjson::SizeType length, bool copy) override;
  bool EndObject(rapidjson::SizeType memberCount) override;
  bool StartArray() override;
  bool EndArray(rapidjson::SizeType elementCount) override;
  void UpdateProgress(size_t aProcessedBytes, size_t aTotalBytes) override;
  void Completed(const resultType& aCompletionResult) override;

  void setPercentCompleteCallback(std::function<void(uint16_t)> callback);
  void setRequestProcessCompleteCallback(
      std::function<void(const resultType&)> callback);

 private:
  bool isProcessingEi = false;
  const EntityIdCallback entityIdCallback;
  std::function<void(uint16_t)> mPercentCompleteCallback;
  uint16_t mPercentComplete = 0;
  std::function<void(const resultType&)> mRequestProcessCompleteCallback;
};

}  // namespace UI
