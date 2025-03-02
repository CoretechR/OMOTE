#pragma once

#include <rapidjson/document.h>

#include "HomeAssist/WebSocket/Message/Message.hpp"

namespace HomeAssist::WebSocket {

class Message::State {
 public:
  State(const rapidjson::Value& aStateValue);
  virtual ~State();

 private:
  void SaveBasicInfo(const rapidjson::Value& aStateValue);
  void SaveAttributes(const rapidjson::Value& aAttributesListValue);

  std::string mEntityId;
  std::string mState;
  std::unique_ptr<Attributes> mAttributes;
};

}  // namespace HomeAssist::WebSocket
