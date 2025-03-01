#include "HomeAssist/WebSocketMessage.hpp"

#include <map>
#include <string>

namespace HomeAssist {

static std::map<std::string, WebSocketMessage::Type> typeMap = {
    {"auth", WebSocketMessage::Type::auth},
    {"auth_required", WebSocketMessage::Type::auth_required},
    {"auth_ok", WebSocketMessage::Type::auth_ok},
    {"auth_invalid", WebSocketMessage::Type::auth_invalid},
    {"event", WebSocketMessage::Type::event},
    {"result", WebSocketMessage::Type::result}};

WebSocketMessage::WebSocketMessage(const rapidjson::Document& messageJson) {
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

WebSocketMessage::~WebSocketMessage() {
  // Destructor implementation
}

}  // namespace HomeAssist