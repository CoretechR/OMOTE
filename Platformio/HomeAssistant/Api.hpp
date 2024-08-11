#pragma once
#include "curl/curl.h"
#include "rapidjson/document.h"
#include <memory>
#include <string>

namespace HomeAssistant {

class Api {
public:
  Api(std::string aIp = HOMEASSISTANT_IP_ADDRESS,
      std::string aPort = HOMEASSISTANT_PORT);
  virtual ~Api();

  bool IsApiRunning();
  rapidjson::Document SendRequest(std::string anApiPath = "");

private:
  static constexpr auto jsonHeader = "Content-Type: application/json";

  std::string getSecurityHeader();
  /**
   * @brief Take base URL http://IP:PORT/api/ and add some
   *
   * @param aPath - Path To Append
   * @return std::string - Full path to API endpoint
   */
  std::string getURL(std::string aPath = "");

  static size_t HandleResultCallback(void *contents, size_t size, size_t nmemb,
                                     void *userp);

  const std::string mIp;
  const std::string mPort;

  std::string mResponse = "";
};

} // namespace HomeAssistant