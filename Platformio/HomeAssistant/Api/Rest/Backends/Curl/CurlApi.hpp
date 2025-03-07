#pragma once
#include <functional>
#include <memory>
#include <string>

#include "HomeAssistant/Api/Rest/Access/IHomeAssistApi.hpp"
#include "curl/curl.h"

namespace HomeAssist {

class CurlApi : public IHomeAssistApi {
 public:
  using AutoCleanupCurl = std::unique_ptr<CURL, std::function<void(CURL *)>>;

  CurlApi(std::string aIp = HOMEASSISTANT_IP_ADDRESS,
          std::string aPort = HOMEASSISTANT_PORT);
  virtual ~CurlApi();

  // Send update to Home Assistant and get response json back
  std::string SendUpdate(const std::string &anApiPath,
                         const std::string &aUpdateJson) override;

  std::string SendRequest(const std::string &anApiPath = "") override;

 private:
  static constexpr auto jsonHeader = "Content-Type: application/json";

  std::string Send(AutoCleanupCurl aCurl);
  AutoCleanupCurl SetupEasyCurl(const std::string &aURL);
  auto SetupHeaderList();
  void AddStandardRequestData(CURL *aCurl, curl_slist *aHeaders);

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

}  // namespace HomeAssist