#pragma once
#include "HomeAssistApiParser.hpp"
#include "rapidjson/document.h"
#include <functional>
#include <memory>
#include <string>

namespace HomeAssist {

class IHomeAssistApi {
public:
  virtual ~IHomeAssistApi() = default;

  bool IsApiRunning() {
    std::string apiPath = "";
    auto doc = SendRequest(apiPath);

    HomeAssistApiParser::HandlerMapType handlerMap;

    handlerMap["message"] = [](auto &aValue) {
      if (aValue.IsString()) {
        return aValue.GetString() == std::string("API running.");
      }
      return false;
    };

    HomeAssistApiParser parser(handlerMap);
    rapidjson::StringStream s(doc.c_str());
    rapidjson::Reader reader;
    return reader.Parse(s, parser);
  }

  virtual std::string SendUpdate(const std::string &anApiPath,
                                 const std::string &aUpdateJson) = 0;

  virtual std::string SendRequest(const std::string &anApiPath) = 0;
};

} // namespace HomeAssist