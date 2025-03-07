#pragma once
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include <algorithm>
#include <functional>
#include <map>
#include <string>

namespace HomeAssist {
/**
 * @brief Class made to simplify creating parsers inline
 *        uses HandlerMapType to run functions based on
 *        keys.
 */
class HomeAssistApiParser
    : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>,
                                          HomeAssistApiParser> {
public:
  /**
   *   Map Key values to functions that should be ran when
   *   the key is found on the value at the key.
   */
  using HandlerMapType =
      std::map<std::string, std::function<bool(rapidjson::Value &)>>;

  HomeAssistApiParser() {}
  HomeAssistApiParser(HandlerMapType aMapOfHandlers)
      : mKeyHandlers(aMapOfHandlers){};
  ~HomeAssistApiParser();

  HandlerMapType &GetHandlersRef() { return mKeyHandlers; }

  bool StartObject() { return true; }
  bool EndObject(size_t memberCount) { return true; }
  bool StartArray() { return true; }
  bool EndArray(size_t elementCount) { return true; }

  bool Key(const char *str, rapidjson::SizeType length, bool copy) {
    std::string key(str);
    mFoundKeyHandler =
        std::find_if(mKeyHandlers.begin(), mKeyHandlers.end(),
                     [key](auto handler) { return handler.first == key; });
    return true;
  }

  bool String(const char *str, rapidjson::SizeType length, bool copy) {
    if (mFoundKeyHandler != mKeyHandlers.end()) {
      rapidjson::Value value(rapidjson::kStringType);
      value.SetString(str, length);
      return mFoundKeyHandler->second(value);
    }
    return true;
  }
  bool Int(int i) { return true; }
  bool Uint(unsigned u) {
    if (mFoundKeyHandler != mKeyHandlers.end()) {
      rapidjson::Value value(rapidjson::kNumberType);
      value.SetUint(u);
      return mFoundKeyHandler->second(value);
    }
    return true;
  }
  bool Int64(int64_t i) { return true; }
  bool Uint64(int64_t u) { return true; }
  bool Double(double d) { return true; }
  bool Bool(bool b) { return true; }
  bool Null() { return true; }

private:
  HandlerMapType mKeyHandlers;
  HandlerMapType::iterator mFoundKeyHandler;
};

inline HomeAssistApiParser::~HomeAssistApiParser() {}

} // namespace HomeAssist