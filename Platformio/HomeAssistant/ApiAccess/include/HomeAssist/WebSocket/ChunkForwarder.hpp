#pragma once
#include <memory>

#include "websockets/IChunkProcessor.hpp"

namespace HomeAssist::WebSocket {

class Api;

class ChunkForwarder : public HAL::WebSocket::IChunkProcessor {
 public:
  ChunkForwarder(Api& aApi);
  virtual ~ChunkForwarder() = default;

  void SetProcessor(
      std::shared_ptr<HAL::WebSocket::IChunkProcessor> aProcessor);

 protected:
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
  Api& mApi;
  std::weak_ptr<HAL::WebSocket::IChunkProcessor> mProcessor;
};

}  // namespace HomeAssist::WebSocket
