#pragma once
#include "HomeAssist/IHomeAssistApi.hpp"

namespace HomeAssist {

class HomeAssistantAccess {
public:
  static std::shared_ptr<IHomeAssistApi> GetApiAccess();

private:
  static std::weak_ptr<IHomeAssistApi> mCurrentApiAccess;
};

} // namespace HomeAssist