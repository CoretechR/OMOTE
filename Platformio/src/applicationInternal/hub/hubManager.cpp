#include "hubManager.h"
#include "hubBackendBase.h"
#include "applicationInternal/omote_log.h"
#include <Arduino.h>

#if (ENABLE_ESPNOW == 1)
#include "espnowBackend.h"
#endif

#if (ENABLE_WIFI_AND_MQTT == 1)
#include "mqttBackend.h"
#endif

HubManager& HubManager::getInstance() {
  static HubManager instance;
  return instance;
}

HubManager::HubManager() : currentBackend(HubBackend::ESPNOW) {
  // Initialize with default backend type only
}

std::unique_ptr<HubBackendBase> HubManager::createBackend(HubBackend backend) {
  switch (backend) {
    case HubBackend::ESPNOW:
      #if (ENABLE_ESPNOW == 1)
      return std::unique_ptr<HubBackendBase>(new EspNowBackend());
      #else
      omote_log_e("ESP-NOW backend is not available in this build\n");
      return nullptr;
      #endif
    case HubBackend::MQTT:
      #if (ENABLE_WIFI_AND_MQTT == 1)
      return std::unique_ptr<HubBackendBase>(new MqttBackend());
      #else
      omote_log_e("MQTT backend is not available in this build\n");
      return nullptr;
      #endif
    default:
      omote_log_e("Invalid hub backend selected\n");
      return nullptr;
  }
}

bool HubManager::init(HubBackend backend) {
  // Store the selected backend type
  currentBackend = backend;

  // Create the new backend
  auto newBackend = createBackend(backend);
  if (!newBackend) {
    return false;
  }

  // Initialize the backend
  if (!newBackend->init()) {
    omote_log_e("Failed to initialize hub backend\n");
    return false;
  }

  // If everything succeeded, replace the active backend
  activeBackend = std::move(newBackend);
  return true;
}

void HubManager::process() {
  if (activeBackend) {
    activeBackend->process();
  }
}

bool HubManager::sendMessage(const json& payload) {
  if (!activeBackend) {
    omote_log_w("Cannot send message: no hub backend initialized\n");
    return false;
  }
  
  if (!activeBackend->isReady()) {
    omote_log_w("Cannot send message: hub backend not ready\n");
    return false;
  }
  
  return activeBackend->sendMessage(payload);
}

bool HubManager::isInitialized() const {
  return activeBackend != nullptr;
}

bool HubManager::isReady() const {
  return activeBackend && activeBackend->isReady();
}

void HubManager::shutdown() {
  if (activeBackend) {
    activeBackend->shutdown();
    activeBackend.reset();
  }
}

HubBackend HubManager::getCurrentBackend() const {
  return currentBackend;
} 