#pragma once
#include "HomeAssist/IHomeAssistApi.hpp"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <memory>
#include <optional>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

using namespace rapidjson;

namespace HomeAssist {

class HttpClientApi : public IHomeAssistApi {
public:
  HttpClientApi(const std::string &ip = HOMEASSISTANT_IP_ADDRESS,
                const std::string &port = HOMEASSISTANT_PORT)
      : mIp(ip), mPort(port) {}
  virtual ~HttpClientApi() = default;

  std::string SendRequest(const std::string &apiPath) override {
    HTTPClient http;
    http.begin(String(getURL(apiPath).c_str()));

    http.addHeader("Authorization",
                   "Bearer " + String(HOMEASSISTANT_API_TOKEN));
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
      http.end();
      return "";
    }

    String payload = http.getString();
    http.end();

    Serial.println("Inbound from get");
    Serial.print(payload);
    Serial.print("\n\n");

    return std::string(payload.c_str());
  }

  std::string SendUpdate(const std::string &apiPath,
                         const std::string &updateJson) override {
    HTTPClient http;
    http.begin(String(getURL(apiPath).c_str()));

    http.addHeader("Authorization",
                   "Bearer " + String(HOMEASSISTANT_API_TOKEN));
    http.addHeader("Content-Type", "application/json");

    int httpCode =
        http.POST(updateJson.c_str()); // Convert std::string to C string
    if (httpCode != HTTP_CODE_OK) {
      http.end();
      return "";
    }

    String payload = http.getString();
    http.end();

    Serial.println("Inbound from post");
    Serial.print(payload);
    Serial.print("\n\n");

    return std::string(payload.c_str());
  }

private:
  std::string getURL(const std::string &path) {
    return "http://" + mIp + ":" + mPort + "/api/" + path;
  }

  std::string mIp;
  std::string mPort;
};

} // namespace HomeAssist