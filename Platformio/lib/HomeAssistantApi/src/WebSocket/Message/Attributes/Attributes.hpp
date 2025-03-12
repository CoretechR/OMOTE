#pragma once
#include "RapidJsonUtilty.hpp"
#include "WebSocket/Message/Message.hpp"

namespace HomeAssist::WebSocket {

class Message::Attributes {
 public:
  class Light;
  enum class EntityType { Unknown, Light };

  Attributes(EntityType aEntityType, const MemConciousValue& aAttributeVal);
  ~Attributes();

  Light* BorrowLight();

 private:
  std::unique_ptr<Light> mLightAttributes;
};

}  // namespace HomeAssist::WebSocket
