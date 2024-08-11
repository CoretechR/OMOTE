#include "Api.hpp"

#include <memory>
using namespace HomeAssistant;

Api::Api(std::string aIp, std::string aPort) : mIp(aIp), mPort(aPort) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

Api::~Api() { curl_global_cleanup(); }

bool Api::IsApiRunning() {
  auto res = SendRequest();

  if (res.HasMember("message") && res["message"].IsString()) {
    return strcmp(res["message"].GetString(), "API running.") == 0;
  }
  return false;
}

rapidjson::Document Api::SendRequest(std::string anApiPath) {
  // Curl Easy Init
  auto curlEasyCleanup = [&](CURL *aFinishedCurl) {
    curl_easy_cleanup(aFinishedCurl);
  };
  std::unique_ptr<CURL, decltype(curlEasyCleanup)> easyCurl(curl_easy_init(),
                                                            curlEasyCleanup);

  // Curl Create Headers
  struct curl_slist *headersPtr = NULL;
  headersPtr = curl_slist_append(headersPtr, jsonHeader);
  headersPtr = curl_slist_append(headersPtr, getSecurityHeader().c_str());

  auto listDeleter = [&](curl_slist *ptr) { curl_slist_free_all(ptr); };
  std::unique_ptr<curl_slist, decltype(listDeleter)> headers(headersPtr,
                                                             listDeleter);

  // Put together Curl Request
  curl_easy_setopt(easyCurl.get(), CURLOPT_URL, getURL(anApiPath).c_str());
  curl_easy_setopt(easyCurl.get(), CURLOPT_HTTPHEADER, headers.get());
  curl_easy_setopt(easyCurl.get(), CURLOPT_WRITEFUNCTION,
                   Api::HandleResultCallback);
  curl_easy_setopt(easyCurl.get(), CURLOPT_WRITEDATA, this);
  auto code = curl_easy_perform(easyCurl.get());

  printf("Curl Code:%d", code);

  rapidjson::Document d;
  d.Parse(mResponse.c_str());

  return d;
}

std::string Api::getSecurityHeader() {
  return "Authorization: Bearer " + std::string(HOMEASSISTANT_API_TOKEN);
}

std::string Api::getURL(std::string aPath) {
  return "http://" + mIp + ":" + mPort + "/api/" + aPath;
}

// Static
size_t Api::HandleResultCallback(void *contents, size_t size, size_t nmemb,
                                 void *userp) {
  size_t total_size = size * nmemb;
  printf("%s", (char *)contents); // Print the received data

  static_cast<Api *>(userp)->mResponse =
      std::string((char *)contents, total_size);

  return total_size; // Return the number of bytes handled
}