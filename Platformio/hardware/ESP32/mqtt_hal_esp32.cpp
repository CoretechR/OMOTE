#include "WiFi.h"
#include <PubSubClient.h>
#include "mqtt_hal_esp32.h"
#include "secrets.h"

#if (ENABLE_WIFI_AND_MQTT == 1)
WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool isWifiConnected = false;

showWiFiconnected_cb thisShowWiFiconnected_cb = NULL;
void set_showWiFiconnected_cb_HAL(showWiFiconnected_cb pShowWiFiconnected_cb) {
  thisShowWiFiconnected_cb = pShowWiFiconnected_cb;  
}

bool getIsWifiConnected_HAL() {
  return isWifiConnected;
}

// WiFi status event
void WiFiEvent(WiFiEvent_t event){
  //Serial.printf("[WiFi-event] event: %d\r\n", event);
  if(event == ARDUINO_EVENT_WIFI_STA_GOT_IP){
    // connection to MQTT server will be done in checkMQTTconnection()
    // mqttClient.setServer(MQTT_SERVER, 1883); // MQTT initialization
    // mqttClient.connect("OMOTE"); // Connect using a client id

  }

  // Set status bar icon based on WiFi status
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP || event == ARDUINO_EVENT_WIFI_STA_GOT_IP6) {
    isWifiConnected = true;
    thisShowWiFiconnected_cb(true);
    Serial.printf("WiFi connected, IP address: %s\r\n", WiFi.localIP().toString().c_str());

  } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
    isWifiConnected = false;
    thisShowWiFiconnected_cb(false);
    // automatically try to reconnect
    Serial.printf("WiFi got disconnected. Will try to reconnect.\r\n");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  } else {
    // e.g. ARDUINO_EVENT_WIFI_STA_CONNECTED or many others
    // connected is not enough, will wait for IP
    isWifiConnected = false;
    thisShowWiFiconnected_cb(false);

  }
}

void init_mqtt_HAL(void) {
  // Setup WiFi
  WiFi.setHostname("OMOTE"); //define hostname
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(true);
}

bool checkMQTTconnection() {

  if (WiFi.isConnected()) {
    if (mqttClient.connected()) {
      return true;
    } else {
      // try to connect to mqtt server
      mqttClient.setServer(MQTT_SERVER, MQTT_SERVER_PORT); // MQTT initialization
      if (mqttClient.connect(MQTT_CLIENTNAME, MQTT_USER, MQTT_PASS)) {
        Serial.printf("  Successfully connected to MQTT broker\r\n");
    
      } else {
        Serial.printf("  MQTT connection failed (but WiFi is available). Will try later ...\r\n");

      }
      return mqttClient.connected();
    }
  } else {
    Serial.printf("  No connection to MQTT server, because WiFi ist not connected.\r\n");
    return false;
  }  
}

bool publishMQTTMessage_HAL(const char *topic, const char *payload){

  if (checkMQTTconnection()) {
    // Serial.printf("Sending mqtt payload to topic \"%s\": %s\r\n", topic, payload);
      
    if (mqttClient.publish(topic, payload)) {
      // Serial.printf("Publish ok\r\n");
      return true;
    }
    else {
      Serial.printf("Publish failed\r\n");
    }
  } else {
    Serial.printf("  Cannot publish mqtt message, because checkMQTTconnection failed (WiFi or mqtt is not connected)\r\n");
  }
  return false;
}

void wifiStop_HAL() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

#endif
