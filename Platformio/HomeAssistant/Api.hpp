#pragma once
#include "curl/curl.h"
#include "rapidjson/document.h"
#include <string>
namespace HomeAssistant {

class Api {
public:
  Api();
  virtual ~Api();

  rapidjson::Document doStuff();

private:
  static constexpr auto jsonHeader = "Content-Type: application/json";
  static constexpr auto token = "nope :)";

  std::string getSecurityHeader();
  std::string getURL();

  // Callback function to handle data received from the server
  static size_t write_callback(void *contents, size_t size, size_t nmemb,
                               void *userp);

  std::string mIp = "192.168.86.105";
  std::string mPort = "8123";

  std::string mResponse = "";

  CURL *mCurlHandle = nullptr;
};

} // namespace HomeAssistant