/*
  If you add additional overrides here, you have to
  1. first add #undef
  2. add new #define
*/
#undef WIFI_SSID
#undef WIFI_PASSWORD
#undef MQTT_SERVER

#define WIFI_SSID                  "YourWifiSSID"              // override here
#define WIFI_PASSWORD              "YourWifiPassword"          // override here
#define MQTT_SERVER                "IPAddressOfYourBroker"     // override here

// Hub Communication Configuration
#define ENABLE_HUB_COMMUNICATION 1

// Hub Backend Configuration
// 0 = MQTT, 1 = ESP-NOW
#define PREFERRED_HUB_BACKEND 1  // Default to ESP-NOW

// MQTT Configuration (if using MQTT backend)
#define MQTT_SERVER "192.168.1.100"
#define MQTT_SERVER_PORT 1883
#define MQTT_USER "username"
#define MQTT_PASS "password"
#define MQTT_CLIENTNAME "OMOTE"

// ESP-NOW Configuration (if using ESP-NOW backend)
#define ESPNOW_HUB_MAC {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}
