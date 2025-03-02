#include "HomeAssist/WebSocket/Message/State.hpp"

#include "HomeAssist/WebSocket/Message/Attributes/Attributes.hpp"

using namespace HomeAssist::WebSocket;

Message::State::State(const rapidjson::Value& aStateValue) {
  SaveBasicInfo(aStateValue);
  if (aStateValue.HasMember("attributes") &&
      aStateValue["attributes"].IsObject()) {
    SaveAttributes(aStateValue["attributes"]);
  }
}

Message::State::~State() {}

void Message::State::SaveBasicInfo(const rapidjson::Value& aStateValue) {
  if (aStateValue.HasMember("entity_id") &&
      aStateValue["entity_id"].IsString()) {
    mEntityId = aStateValue["entity_id"].GetString();
  }
  if (aStateValue.HasMember("state") && aStateValue["state"].IsString()) {
    mState = aStateValue["state"].GetString();
  }
}

void Message::State::SaveAttributes(
    const rapidjson::Value& aAttributesListValue) {
  if (mEntityId.find("light") == 0) {
    mAttributes = std::make_unique<Attributes>(Attributes::EntityType::Light,
                                               aAttributesListValue);
  }
}