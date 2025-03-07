#pragma once

#include "hubBackendBase.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#if (ENABLE_ESPNOW == 1)
class EspNowBackend : public HubBackendBase {
public:
  EspNowBackend();
  ~EspNowBackend() override;
  
  bool init() override;
  void process() override;
  bool sendMessage(const json& payload) override;
  bool isReady() override;
  void shutdown() override;
};
#endif 