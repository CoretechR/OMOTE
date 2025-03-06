#include "hubManager.h"
#include "hubInterface.h"
#include "applicationInternal/omote_log.h"
#include <Arduino.h>
#include "espnowBackend.h"

#if (ENABLE_WIFI_AND_MQTT == 1)
#include "mqttBackend.h"
#endif

HubManager& HubManager::getInstance() {
  static HubManager instance;
  return instance;
}

HubManager::HubManager() : currentBackend(HubBackend::ESPNOW), activeBackend(nullptr) {
  // Initialize with default backend
}

HubManager::~HubManager() {
  if (activeBackend != nullptr) {
    activeBackend->shutdown();
    delete activeBackend;
    activeBackend = nullptr;
  }
}

bool HubManager::init(HubBackend backend) {
  // Clean up any existing backend
  if (activeBackend != nullptr) {
    activeBackend->shutdown();
    delete activeBackend;
    activeBackend = nullptr;
  }

  // Store the selected backend type
  currentBackend = backend;

  // Initialize the selected backend
  switch (backend) {
    case HubBackend::ESPNOW:
      activeBackend = new EspNowBackend();
      break;
    case HubBackend::MQTT:
      #if (ENABLE_WIFI_AND_MQTT == 1)
      activeBackend = new MqttBackend();
      #else
      omote_log_e("MQTT backend is not available in this build\n");
      return false;
      #endif
      break;
    default:
      omote_log_e("Invalid hub backend selected\n");
      return false;
  }

  // Initialize the backend
  if (!activeBackend->init()) {
    omote_log_e("Failed to initialize hub backend\n");
    delete activeBackend;
    activeBackend = nullptr;
    return false;
  }

  return true;
}

void HubManager::process() {
  if (activeBackend != nullptr) {
    activeBackend->process();
  }
}

bool HubManager::sendMessage(const char* device, const char* command, json payload) {
  if (activeBackend == nullptr) {
    omote_log_w("Cannot send message: no hub backend initialized\n");
    return false;
  }
  
  if (!activeBackend->isReady()) {
    omote_log_w("Cannot send message: hub backend not ready\n");
    return false;
  }
  
  return activeBackend->sendMessage(device, command, payload);
}

bool HubManager::isInitialized() const {
  return activeBackend != nullptr;
}

bool HubManager::isReady() const {
  if (activeBackend == nullptr) {
    return false;
  }
  return activeBackend->isReady();
}

void HubManager::shutdown() {
  if (activeBackend != nullptr) {
    activeBackend->shutdown();
    delete activeBackend;
    activeBackend = nullptr;
  }
}

HubBackend HubManager::getCurrentBackend() const {
  return currentBackend;
} 