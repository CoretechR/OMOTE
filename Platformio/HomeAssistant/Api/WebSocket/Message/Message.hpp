#pragma once

#include <string>
#include <vector>

#include "RapidJsonUtilty.hpp"

namespace HomeAssist::WebSocket {

class Message {
 public:
  class Entity;
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

  Message(const MemConciousDocument& messageJson);
  virtual ~Message();

  inline Type GetType() const;
  inline Success GetSuccess() const;
  inline int GetId() const;

  Entity* BorrowToState() const;
  Entity* BorrowFromState() const;
  const std::vector<std::unique_ptr<Entity>>& BorrowEntityList() const;

 private:
  void SaveBasicInfo(const MemConciousDocument& aMessageJson);
  void SaveStateInfo(const MemConciousDocument& aMessageJson);
  void SaveResultInfo(const MemConciousDocument& aMessageJson);

  int mId = 0;
  Success mSuccess = Success::unknown;
  Type mType = Type::unknown;
  std::unique_ptr<Entity> mFromState;
  std::unique_ptr<Entity> mToState;
  std::vector<std::unique_ptr<Entity>> mEntityList;
};

inline Message::Type Message::GetType() const { return mType; }
inline Message::Success Message::GetSuccess() const { return mSuccess; }
inline int Message::GetId() const { return mId; }

}  // namespace HomeAssist::WebSocket
