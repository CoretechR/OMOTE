#pragma once

#include "hubInterface.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#if (ENABLE_ESPNOW == 1)
class EspNowBackend : public HubInterface {
public:
  EspNowBackend();
  virtual ~EspNowBackend();
  
  bool init() override;
  void process() override;
  bool sendMessage(const char* device, const char* command, json payload) override;
  bool isReady() override;
  void shutdown() override;
};
#endif 