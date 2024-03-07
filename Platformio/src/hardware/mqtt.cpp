#include <PubSubClient.h>
#include <lvgl.h>
#include "hardware/mqtt.h"
#include "gui_general_and_keys/guiBase.h"
#include "secrets.h"
#include "commandHandler.h"

#ifdef ENABLE_WIFI_AND_MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

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
    if (WifiLabel != NULL) {lv_label_set_text(WifiLabel, LV_SYMBOL_WIFI);}

  } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
    if (WifiLabel != NULL) {lv_label_set_text(WifiLabel, "");}
    // automatically try to reconnect
    Serial.printf("WiFi got disconnected. Will try to reconnect.\r\n");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  } else {
    // e.g. ARDUINO_EVENT_WIFI_STA_CONNECTED or many others
    // connected is not enough, will wait for IP
    if (WifiLabel != NULL) {lv_label_set_text(WifiLabel, "");}

  }
}

void init_mqtt(void) {
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

bool publishMQTTMessage(const char *topic, const char *payload){

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
#endif
