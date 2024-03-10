#include "mqtt_hal_windows_linux.h"
#if (ENABLE_WIFI_AND_MQTT == 1)

bool getIsWifiConnected_HAL() {
  return true;
}

void init_mqtt_HAL(void) {}

bool publishMQTTMessage_HAL(const char *topic, const char *payload){
  return false;
}

void wifiStop_HAL() {}

void set_showWiFiconnected_cb_HAL(showWiFiconnected_cb pShowWiFiconnected_cb) {}

#endif
