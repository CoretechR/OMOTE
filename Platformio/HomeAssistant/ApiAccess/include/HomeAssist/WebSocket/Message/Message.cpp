#include <map>
#include <string>

#include "HomeAssist/WebSocket/Message/Message.hpp"

namespace HomeAssist::WebSocket {

static std::map<std::string, Message::Type> typeMap = {
    {"auth", Message::Type::auth},
    {"auth_required", Message::Type::auth_required},
    {"auth_ok", Message::Type::auth_ok},
    {"auth_invalid", Message::Type::auth_invalid},
    {"event", Message::Type::event},
    {"result", Message::Type::result}};

Message::Message(const rapidjson::Document& messageJson) {
  if (messageJson.HasMember("id") && messageJson["id"].IsInt()) {
    mId = messageJson["id"].GetInt();
  }
  if (messageJson.HasMember("type") && messageJson["type"].IsString()) {
    mType = typeMap[messageJson["type"].GetString()];
  }
  if (messageJson.HasMember("success") && messageJson["success"].IsBool()) {
    mSuccess =
        messageJson["success"].GetBool() ? Success::success : Success::failure;
  }
}

Message::~Message() {
  // Destructor implementation
}

}  // namespace HomeAssist::WebSocket