#include "HomeAssist/WebSocket/Message/Message.hpp"

#include <map>
#include <string>

#include "HomeAssist/WebSocket/Message/State.hpp"
#include "RapidJsonUtilty.hpp"

namespace HomeAssist::WebSocket {

static std::map<std::string, Message::Type> typeMap = {
    {"auth", Message::Type::auth},
    {"auth_required", Message::Type::auth_required},
    {"auth_ok", Message::Type::auth_ok},
    {"auth_invalid", Message::Type::auth_invalid},
    {"event", Message::Type::event},
    {"result", Message::Type::result}};

Message::Message(const rapidjson::Document& aMessageJson) {
  SaveBasicInfo(aMessageJson);
  if (mType == Type::event) {
    SaveStateInfo(aMessageJson);
  }
}

Message::~Message() {};

void Message::SaveBasicInfo(const rapidjson::Document& aMessageJson) {
  if (aMessageJson.HasMember("id") && aMessageJson["id"].IsInt()) {
    mId = aMessageJson["id"].GetInt();
  }
  if (aMessageJson.HasMember("type") && aMessageJson["type"].IsString()) {
    mType = typeMap[aMessageJson["type"].GetString()];
  }
  if (aMessageJson.HasMember("success") && aMessageJson["success"].IsBool()) {
    mSuccess =
        aMessageJson["success"].GetBool() ? Success::success : Success::failure;
  }
}

void Message::SaveStateInfo(const rapidjson::Document& aMessageJson) {
  if (auto oldStateVal = GetNestedField(
          aMessageJson, {"event", "variables", "trigger", "from_state"});
      oldStateVal) {
    mFromState = std::make_unique<State>(*oldStateVal);
  }
  if (auto newStateVal = GetNestedField(
          aMessageJson, {"event", "variables", "trigger", "to_state"});
      newStateVal) {
    mToState = std::make_unique<State>(*newStateVal);
  }
}

}  // namespace HomeAssist::WebSocket