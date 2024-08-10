#include "Api.hpp"

using namespace HomeAssistant;

Api::Api() { curl_global_init(CURL_GLOBAL_DEFAULT); }

Api::~Api() { curl_global_cleanup(); }

rapidjson::Document Api::doStuff() {
  mCurlHandle = curl_easy_init();
  struct curl_slist *headers = NULL; // List to hold headers

  headers = curl_slist_append(headers, jsonHeader);
  headers = curl_slist_append(headers, getSecurityHeader().c_str());

  curl_easy_setopt(mCurlHandle, CURLOPT_URL, getURL().c_str());
  curl_easy_setopt(mCurlHandle, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(mCurlHandle, CURLOPT_WRITEFUNCTION, Api::write_callback);
  curl_easy_setopt(mCurlHandle, CURLOPT_WRITEDATA, this);
  auto code = curl_easy_perform(mCurlHandle);

  printf("Curl Code:%d", code);

  rapidjson::Document d;
  d.Parse(mResponse.c_str());

  curl_slist_free_all(headers);
  curl_easy_cleanup(mCurlHandle);

  return d;
}

std::string Api::getSecurityHeader() {
  return "Authorization: Bearer " + std::string(token);
}

std::string Api::getURL() { return "http://" + mIp + ":" + mPort + "/api/"; }

// Static
size_t Api::write_callback(void *contents, size_t size, size_t nmemb,
                           void *userp) {
  size_t total_size = size * nmemb;
  printf("%s", (char *)contents); // Print the received data

  static_cast<Api *>(userp)->mResponse =
      std::string((char *)contents, total_size);

  return total_size; // Return the number of bytes handled
}