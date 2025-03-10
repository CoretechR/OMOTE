#pragma once
#include <functional>
#include <memory>
#include <string>

#include "Rest/Access/IHomeAssistApi.hpp"
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

CurlApi::CurlApi(std::string aIp, std::string aPort) : mIp(aIp), mPort(aPort) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

CurlApi::~CurlApi() { curl_global_cleanup(); }

CurlApi::AutoCleanupCurl CurlApi::SetupEasyCurl(const std::string &aApiPath) {
  // Curl Easy Init
  auto curlEasyCleanup = [](CURL *aFinishedCurl) {
    curl_easy_cleanup(aFinishedCurl);
  };
  CurlApi::AutoCleanupCurl easyCurl(curl_easy_init(), curlEasyCleanup);
  auto url = getURL() + aApiPath;
  curl_easy_setopt(easyCurl.get(), CURLOPT_URL, url.c_str());
  return easyCurl;
}

auto CurlApi::SetupHeaderList() {
  // Curl Create Headers
  struct curl_slist *headersPtr = NULL;
  headersPtr = curl_slist_append(headersPtr, jsonHeader);
  headersPtr = curl_slist_append(headersPtr, getSecurityHeader().c_str());

  auto listDeleter = [&](curl_slist *ptr) { curl_slist_free_all(ptr); };
  std::unique_ptr<curl_slist, decltype(listDeleter)> headers(headersPtr,
                                                             listDeleter);
  return headers;
}

void CurlApi::AddStandardRequestData(CURL *aCurl, curl_slist *aHeaders) {
  // Put together Curl Request
  curl_easy_setopt(aCurl, CURLOPT_HTTPHEADER, aHeaders);
  curl_easy_setopt(aCurl, CURLOPT_WRITEFUNCTION, CurlApi::HandleResultCallback);
  curl_easy_setopt(aCurl, CURLOPT_WRITEDATA, this);
}

std::string CurlApi::SendRequest(const std::string &anApiPath) {
  auto easyCurl = SetupEasyCurl(anApiPath);
  auto headers = SetupHeaderList();
  AddStandardRequestData(easyCurl.get(), headers.get());
  return Send(std::move(easyCurl));
}

std::string CurlApi::Send(AutoCleanupCurl aCurl) {
  auto code = curl_easy_perform(aCurl.get());
  return mResponse;
}

std::string CurlApi::SendUpdate(const std::string &anApiPath,
                                const std::string &aUpdateJson) {
  auto easyCurl = SetupEasyCurl(anApiPath);
  auto headers = SetupHeaderList();
  AddStandardRequestData(easyCurl.get(), headers.get());
  curl_easy_setopt(easyCurl.get(), CURLOPT_POSTFIELDS, aUpdateJson.c_str());
  auto res = Send(std::move(easyCurl));
  return res;
}

std::string CurlApi::getSecurityHeader() {
  return "Authorization: Bearer " + std::string(HOMEASSISTANT_API_TOKEN);
}

std::string CurlApi::getURL(std::string aPath) {
  return "http://" + mIp + ":" + mPort + "/api/" + aPath;
}

// Static
size_t CurlApi::HandleResultCallback(void *contents, size_t size, size_t nmemb,
                                     void *userp) {
  size_t total_size = size * nmemb;
  printf("%s", (char *)contents);  // Print the received data

  static_cast<CurlApi *>(userp)->mResponse =
      std::string((char *)contents, total_size);

  return total_size;  // Return the number of bytes handled
}

}  // namespace HomeAssist