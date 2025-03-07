#include "HomeAssistant/Api/WebSocket/Message/Message.hpp"

#include <map>
#include <string>

#include "HomeAssistant/Api/WebSocket/Message/Entity.hpp"
#include "HomeAssistant/RapidJsonUtilty.hpp"

namespace HomeAssist::WebSocket {

static std::map<std::string, Message::Type> typeMap = {
    {"auth", Message::Type::auth},
    {"auth_required", Message::Type::auth_required},
    {"auth_ok", Message::Type::auth_ok},
    {"auth_invalid", Message::Type::auth_invalid},
    {"event", Message::Type::event},
    {"result", Message::Type::result}};

Message::Message(const MemConciousDocument& aMessageJson) {
  SaveBasicInfo(aMessageJson);
  switch (mType) {
    case Type::event:
      SaveStateInfo(aMessageJson);
      break;
    case Type::result:
      SaveResultInfo(aMessageJson);
      break;
  }
}

Message::~Message() {};

void Message::SaveBasicInfo(const MemConciousDocument& aMessageJson) {
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

void Message::SaveStateInfo(const MemConciousDocument& aMessageJson) {
  if (auto oldStateVal = GetNestedField(
          aMessageJson, {"event", "variables", "trigger", "from_state"});
      oldStateVal) {
    mFromState = std::make_unique<Entity>(*oldStateVal);
  }
  if (auto newStateVal = GetNestedField(
          aMessageJson, {"event", "variables", "trigger", "to_state"});
      newStateVal) {
    mToState = std::make_unique<Entity>(*newStateVal);
  }
}

void Message::SaveResultInfo(const MemConciousDocument& aMessageJson) {
  if (aMessageJson.HasMember("result") && aMessageJson["result"].IsArray()) {
    const auto& resultArray = aMessageJson["result"].GetArray();
    for (const auto& item : resultArray) {
      if (item.IsObject() && item.HasMember("entity_id")) {
        mEntityList.push_back(std::make_unique<Entity>(item));
      }
    }
  }
}

Message::Entity* Message::BorrowToState() const { return mToState.get(); }
Message::Entity* Message::BorrowFromState() const { return mFromState.get(); }
const std::vector<std::unique_ptr<Message::Entity>>& Message::BorrowEntityList()
    const {
  return mEntityList;
};

}  // namespace HomeAssist::WebSocket