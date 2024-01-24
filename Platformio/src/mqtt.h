#ifndef __MQTT_H__
#define __MQTT_H__

#include "commandHandler.h"

#ifdef ENABLE_WIFI_AND_MQTT

#include "WiFi.h"
#include <PubSubClient.h>

void init_mqtt(void);
bool publishMQTTMessage(const char *topic, const char *payload);
#endif

#endif /*__MQTT_H__*/
