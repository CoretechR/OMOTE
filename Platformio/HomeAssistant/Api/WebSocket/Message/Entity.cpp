#include "HomeAssistant/Api/WebSocket/Message/Entity.hpp"

#include "HomeAssistant/Api/WebSocket/Message/Attributes/Attributes.hpp"

using namespace HomeAssist::WebSocket;

Message::Entity::Entity(const MemConciousValue& aStateValue) {
  SaveBasicInfo(aStateValue);
  if (aStateValue.HasMember("attributes") &&
      aStateValue["attributes"].IsObject()) {
    SaveAttributes(aStateValue["attributes"]);
  }
}

Message::Entity::~Entity() {}

std::string Message::Entity::GetId() { return mEntityId; }

std::string Message::Entity::GetState() { return mState; }

Message::Attributes* Message::Entity::BorrowAttributes() {
  return mAttributes.get();
}
void Message::Entity::SaveBasicInfo(const MemConciousValue& aStateValue) {
  if (aStateValue.HasMember("entity_id") &&
      aStateValue["entity_id"].IsString()) {
    mEntityId = aStateValue["entity_id"].GetString();
  }
  if (aStateValue.HasMember("state") && aStateValue["state"].IsString()) {
    mState = aStateValue["state"].GetString();
  }
}

void Message::Entity::SaveAttributes(
    const MemConciousValue& aAttributesListValue) {
  if (mEntityId.find("light") == 0) {
    mAttributes = std::make_unique<Attributes>(Attributes::EntityType::Light,
                                               aAttributesListValue);
  }
}