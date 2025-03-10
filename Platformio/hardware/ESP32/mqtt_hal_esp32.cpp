#include "mqtt_hal_esp32.h"
#include "WiFi.h"
#include "secrets.h"
#include <PubSubClient.h>

#if (ENABLE_WIFI_AND_MQTT == 1)
WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool isWifiConnected = false;
std::string id = "";

tAnnounceWiFiconnected_cb thisAnnounceWiFiconnected_cb = NULL;
void set_announceWiFiconnected_cb_HAL(
    tAnnounceWiFiconnected_cb pAnnounceWiFiconnected_cb) {
  thisAnnounceWiFiconnected_cb = pAnnounceWiFiconnected_cb;
}

tAnnounceSubscribedTopics_cb thisAnnounceSubscribedTopics_cb = NULL;
void set_announceSubscribedTopics_cb_HAL(
    tAnnounceSubscribedTopics_cb pAnnounceSubscribedTopics_cb) {
  thisAnnounceSubscribedTopics_cb = pAnnounceSubscribedTopics_cb;
}

bool getIsWifiConnected_HAL() { return isWifiConnected; }

std::string platform_create_id_string(void) {
  uint8_t mac[6];
  char id_string[32];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  sprintf(id_string, "ESP32_%02x%02X%02X", mac[3], mac[4], mac[5]);
  return std::string(id_string);
}

// WiFi status event
void WiFiEvent(WiFiEvent_t event) {
  // Serial.printf("[WiFi-event] event: %d\r\n", event);
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
    // connection to MQTT server will be done in checkMQTTconnection()
    // mqttClient.setServer(MQTT_SERVER, 1883); // MQTT initialization
    // mqttClient.connect("OMOTE"); // Connect using a client id
  }

  // Set status bar icon based on WiFi status
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP ||
      event == ARDUINO_EVENT_WIFI_STA_GOT_IP6) {
    isWifiConnected = true;
    thisAnnounceWiFiconnected_cb(true);

    Serial.printf("[OMOTE][%8lu]: WiFi connected, IP address: %s\r\n",
                  (unsigned long)millis(), WiFi.localIP().toString().c_str());

  } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
    isWifiConnected = false;
    thisAnnounceWiFiconnected_cb(false);
    // automatically try to reconnect
    Serial.printf("WiFi got disconnected. Will try to reconnect.\r\n");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  } else {
    // e.g. ARDUINO_EVENT_WIFI_STA_CONNECTED or many others
    // connected is not enough, will wait for IP
    isWifiConnected = false;
    thisAnnounceWiFiconnected_cb(false);
  }
}

void init_mqtt_HAL(void) {
  Serial.printf("[OMOTE][%8lu]: Starting wifi\r\n", (unsigned long)millis());

  // Setup WiFi
  WiFi.setHostname("OMOTE"); // define hostname
  WiFi.onEvent(WiFiEvent);

  // 10.64.0.208
  IPAddress ip(10, 64, 0, 208);
  // 10.64.0.1
  IPAddress gateway(10, 64, 0, 1);
  // 255.255.254.0
  IPAddress subnet(255, 255, 254, 0);

  // TODO: This prevents NVR lookup
  // WiFi.persistent(false);
  // WiFi.mode(WIFI_STA);

  // TODO: Apply static IP settings
  WiFi.config(ip, gateway, subnet);

  // uint8_t MAC[6] = {0xE4, 0x38, 0x83, 0x3E, 0xE2, 0x3D};

  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 1, MAC, true);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(true);

  id = platform_create_id_string();
}

std::string subscribeTopicOMOTEtest = "OMOTE/test";
// For connecting to one or several BLE clients
std::string subscribeTopicOMOTE_BLEstartAdvertisingForAll =
    "OMOTE/BLE/startAdvertisingForAll";
std::string subscribeTopicOMOTE_BLEstartAdvertisingWithWhitelist =
    "OMOTE/BLE/startAdvertisingWithWhitelist";
std::string subscribeTopicOMOTE_BLEstartAdvertisingDirected =
    "OMOTE/BLE/startAdvertisingDirected";
std::string subscribeTopicOMOTE_BLEstopAdvertising =
    "OMOTE/BLE/stopAdvertising";
std::string subscribeTopicOMOTE_BLEprintConnectedClients =
    "OMOTE/BLE/printConnectedClients";
std::string subscribeTopicOMOTE_BLEdisconnectAllClients =
    "OMOTE/BLE/disconnectAllClients";
std::string subscribeTopicOMOTE_BLEprintBonds = "OMOTE/BLE/printBonds";
std::string subscribeTopicOMOTE_BLEdeleteBonds = "OMOTE/BLE/deleteBonds";

void callback(char *topic, byte *payload, unsigned int length) {
  // handle message arrived
  std::string topicReceived(topic);
  std::string strPayload(reinterpret_cast<const char *>(payload), length);
  Serial.printf("MQTT: received topic %s with payload %s\r\n",
                topicReceived.c_str(), strPayload.c_str());

  if (topicReceived == subscribeTopicOMOTEtest) {
    Serial.printf("MQTT: received topic %s with payload %s\r\n",
                  subscribeTopicOMOTEtest.c_str(), strPayload.c_str());
  }
}

void mqtt_subscribeTopics() {
  mqttClient.setCallback(&callback);

  mqttClient.subscribe(subscribeTopicOMOTEtest.c_str());
  Serial.printf("  Successfully subscribed to MQTT topic %s\r\n",
                subscribeTopicOMOTEtest.c_str());
}

bool checkMQTTconnection() {

  if (WiFi.isConnected()) {
    if (mqttClient.connected()) {
      return true;
    } else {
      // try to connect to mqtt server
      mqttClient.setBufferSize(512); // default is 256
      // mqttClient.setKeepAlive(15);     // default is 15   Client will send
      // MQTTPINGREQ to keep connection alive mqttClient.setSocketTimeout(15);
      // // default is 15   This determines how long the client will wait for
      // incoming data when it expects data to arrive - for example, whilst it
      // is in the middle of reading an MQTT packet.
      mqttClient.setServer(MQTT_SERVER,
                           MQTT_SERVER_PORT); // MQTT initialization

      std::string mqttClientName = std::string(MQTT_CLIENTNAME) + "_esp32_" +
                                   std::string(WiFi.macAddress().c_str());
      if (mqttClient.connect(mqttClientName.c_str(), MQTT_USER, MQTT_PASS)) {

        Serial.printf(
            "[OMOTE][%8lu]: Successfully connected to MQTT broker\r\n",
            (unsigned long)millis());

        mqtt_subscribeTopics();

      } else {
        Serial.printf("  MQTT connection failed (but WiFi is available). Will "
                      "try later ...\r\n");
      }
      return mqttClient.connected();
    }
  } else {
    // Serial.printf("  No connection to MQTT server, because WiFi ist not
    // connected.\r\n");
    return false;
  }
}

unsigned long reconnectInterval = 100;
// in order to do reconnect immediately ...
unsigned long lastReconnectAttempt = millis() - reconnectInterval - 1;
void mqtt_loop_HAL() {
  if (!mqttClient.connected()) {
    unsigned long currentMillis = millis();
    if ((currentMillis - lastReconnectAttempt) > reconnectInterval) {
      lastReconnectAttempt = currentMillis;
      // Attempt to reconnect
      checkMQTTconnection();
    }
  }

  if (mqttClient.connected()) {
    mqttClient.loop();
  }
}

bool publishMQTTMessage_HAL(const char *topic, json payload) {

  if (checkMQTTconnection()) {
    // Serial.printf("Sending mqtt payload to topic \"%s\": %s\r\n", topic,
    // payload);
    payload["remoteId"] = id;

    if (mqttClient.publish(topic, payload.dump().c_str())) {
      // Serial.printf("Publish ok\r\n");
      return true;
    } else {
      Serial.printf("Publish failed\r\n");
    }
  } else {
    Serial.printf("  Cannot publish mqtt message, because checkMQTTconnection "
                  "failed (WiFi or mqtt is not connected)\r\n");
  }
  return false;
}

void wifiStop_HAL() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

#endif
