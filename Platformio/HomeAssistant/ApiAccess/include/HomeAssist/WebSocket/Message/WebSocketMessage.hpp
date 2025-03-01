#pragma once

#include <string>

#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

class WebSocketMessage {
 public:
  enum class Success : int8_t { unknown, success, failure };
  enum class Type : int8_t {
    unknown,
    auth,
    auth_required,
    auth_ok,
    auth_invalid,
    event,
    result
  };

  WebSocketMessage(const rapidjson::Document& messageJson);
  virtual ~WebSocketMessage();

  inline Type GetType() const;
  inline Success GetSuccess() const;
  inline int GetId() const;

 private:
  int mId = 0;
  Success mSuccess = Success::unknown;
  Type mType = Type::unknown;
};

inline WebSocketMessage::Type WebSocketMessage::GetType() const {
  return mType;
}
inline WebSocketMessage::Success WebSocketMessage::GetSuccess() const {
  return mSuccess;
}
inline int WebSocketMessage::GetId() const { return mId; }

}  // namespace HomeAssist::WebSocket
