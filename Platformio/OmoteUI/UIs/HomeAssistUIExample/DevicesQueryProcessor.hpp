#pragma once
#include <functional>
#include <string>

#include "websockets/IChunkProcessor.hpp"

namespace UI {

class DevicesQueryProcessor : public HAL::WebSocket::Json::IChunkProcessor {
 public:
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

 private:
  bool isProcessingEi = false;
  const EntityIdCallback entityIdCallback;
};

}  // namespace UI
