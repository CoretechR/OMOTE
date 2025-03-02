#pragma once

#include <string>

#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

class Message {
 public:
  class State;
  class Attributes;

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

  Message(const rapidjson::Document& messageJson);
  virtual ~Message();

  inline Type GetType() const;
  inline Success GetSuccess() const;
  inline int GetId() const;

 private:
  void SaveBasicInfo(const rapidjson::Document& aMessageJson);
  void SaveStateInfo(const rapidjson::Document& aMessageJson);

  int mId = 0;
  Success mSuccess = Success::unknown;
  Type mType = Type::unknown;
  std::unique_ptr<State> mFromState;
  std::unique_ptr<State> mToState;
};

inline Message::Type Message::GetType() const { return mType; }
inline Message::Success Message::GetSuccess() const { return mSuccess; }
inline int Message::GetId() const { return mId; }

}  // namespace HomeAssist::WebSocket
