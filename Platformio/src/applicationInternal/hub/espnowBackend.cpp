#include "espnowBackend.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "applicationInternal/omote_log.h"

#if (ENABLE_ESPNOW == 1)
EspNowBackend::EspNowBackend() {
}

EspNowBackend::~EspNowBackend() {
  shutdown();
}

bool EspNowBackend::init() {
  init_espnow();
  return true;
}

void EspNowBackend::process() {
  espnow_loop();
}

bool EspNowBackend::sendMessage(const json& payload) {
  // Extract device and command for logging
  std::string device = payload["device"];
  std::string command = payload["command"];
  
  omote_log_d("ESP-NOW: Sending message for device %s, command %s\n", device.c_str(), command.c_str());
  return publishEspNowMessage(payload);
}

bool EspNowBackend::isReady() {
  // ESP-NOW is always ready once initialized
  return true;
}

void EspNowBackend::shutdown() {
  espnow_shutdown();
}
#endif 