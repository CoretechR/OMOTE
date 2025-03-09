#include "HomeAssistant/Api/Rest/Access/HomeAssistAccess.hpp"

#if defined(IS_SIMULATOR)
#include "HomeAssistant/Api/Rest/Backends/Curl/CurlApi.hpp"
#else
#include "HomeAssistant/Api/Rest/Backends/HttpClient/HttpClientApi.hpp"

#endif

using namespace HomeAssist;

std::weak_ptr<IHomeAssistApi> HomeAssistantAccess::mCurrentApiAccess;

std::shared_ptr<IHomeAssistApi> GetNewAccess() {
#if defined(IS_SIMULATOR)
  return std::make_shared<CurlApi>();
#else
  return std::make_shared<HttpClientApi>();
#endif
}

std::shared_ptr<IHomeAssistApi> HomeAssistantAccess::GetApiAccess() {
  if (!mCurrentApiAccess.expired()) {
    return mCurrentApiAccess.lock();
  }
  std::shared_ptr<IHomeAssistApi> apiAccess = GetNewAccess();

  mCurrentApiAccess = apiAccess;
  return apiAccess;
}
