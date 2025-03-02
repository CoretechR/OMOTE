#include "HomeAssist/WebSocket/Message/Attributes/Attributes.hpp"

#include "HomeAssist/WebSocket/Message/Attributes/Light.hpp"

using namespace HomeAssist::WebSocket;

Message::Attributes::Attributes(EntityType aEntityType,
                                const rapidjson::Value& aAttributeVal) {
  switch (aEntityType) {
    case EntityType::Light:
      mLightAttributes = std::make_unique<Light>(aAttributeVal);
      break;
    default:
      break;
  }
}

Message::Attributes::Light* Message::Attributes::BorrowLight() {
  return mLightAttributes.get();
}

Message::Attributes::~Attributes() {}
