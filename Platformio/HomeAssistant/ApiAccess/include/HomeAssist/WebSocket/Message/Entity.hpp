#pragma once

#include <rapidjson/document.h>

#include "HomeAssist/WebSocket/Message/Message.hpp"

namespace HomeAssist::WebSocket {

class Message::Entity {
 public:
  Entity(const MemConciousValue& aStateValue);
  virtual ~Entity();

  std::string GetId();
  std::string GetState();

  Attributes* BorrowAttributes();

 private:
  void SaveBasicInfo(const MemConciousValue& aStateValue);
  void SaveAttributes(const MemConciousValue& aAttributesListValue);

  std::string mEntityId;
  std::string mState;
  std::unique_ptr<Attributes> mAttributes;
};

}  // namespace HomeAssist::WebSocket
