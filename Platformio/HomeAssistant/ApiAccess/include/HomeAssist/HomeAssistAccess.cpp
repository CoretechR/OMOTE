#include "HomeAssist/HomeAssistAccess.hpp"
#include "omoteconfig.h"

#if IS_SIMULATOR
#include "CurlApi.hpp"
#else
#include "HttpClientApi.hpp"
#endif

using namespace HomeAssist;

std::weak_ptr<IHomeAssistApi> HomeAssistantAccess::mCurrentApiAccess;

std::shared_ptr<IHomeAssistApi> GetNewAccess() {
#if IS_SIMULATOR
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
