#pragma once
#include "rapidjson/document.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace HomeAssistant {

class IHomeAssistApi {
public:
  virtual ~IHomeAssistApi() = default;

  bool IsApiRunning() {
    std::string apiPath = "";
    auto doc = SendRequest(apiPath);
    if (!doc.has_value()) {
      return false;
    }
    auto &res = doc.value();
    if (res.HasMember("message") && res["message"].IsString()) {
      auto str = res["message"].GetString();
      return std::string("API running.") == str;
    }
    return false;
  }

  virtual std::optional<rapidjson::Document>
  SendUpdate(const std::string &anApiPath, const std::string &aUpdateJson) = 0;

  virtual std::optional<rapidjson::Document>
  SendRequest(const std::string &anApiPath) = 0;
};

} // namespace HomeAssistant