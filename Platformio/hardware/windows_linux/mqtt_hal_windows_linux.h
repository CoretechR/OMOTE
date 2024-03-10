#pragma once

#if (ENABLE_WIFI_AND_MQTT == 1)

void init_mqtt_HAL(void);
bool getIsWifiConnected_HAL();
bool publishMQTTMessage_HAL(const char *topic, const char *payload);
void wifiStop_HAL();

typedef void (*showWiFiconnected_cb)(bool connected);
void set_showWiFiconnected_cb_HAL(showWiFiconnected_cb pShowWiFiconnected_cb);

#endif
