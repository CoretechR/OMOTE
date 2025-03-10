#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

#if (ENABLE_WIFI_AND_MQTT == 1)

void init_mqtt_HAL(void);
bool getIsWifiConnected_HAL();
void mqtt_loop_HAL();
bool publishMQTTMessage_HAL(const char *topic, json payload);
void wifiStop_HAL();

typedef void (*tAnnounceWiFiconnected_cb)(bool connected);
void set_announceWiFiconnected_cb_HAL(
    tAnnounceWiFiconnected_cb pAnnounceWiFiconnected_cb);
typedef void (*tAnnounceSubscribedTopics_cb)(std::string topic,
                                             std::string payload);
void set_announceSubscribedTopics_cb_HAL(
    tAnnounceSubscribedTopics_cb pAnnounceSubscribedTopics_cb);

#endif
