#include "mqttBackend.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "applicationInternal/omote_log.h"

#if (ENABLE_WIFI_AND_MQTT == 1)
MqttBackend::MqttBackend() : baseTopic("omote/") {
}

MqttBackend::~MqttBackend() {
  shutdown();
}

bool MqttBackend::init() {
  init_mqtt();
  return true;
}

void MqttBackend::process() {
  mqtt_loop();
}

bool MqttBackend::sendMessage(const json& payload) {
  std::string topic = baseTopic + "remote_commands";
  std::string payloadStr = payload.dump();
  
  omote_log_d("MQTT: Sending to topic %s: %s\n", topic.c_str(), payloadStr.c_str());
  return publishMQTTMessage(topic.c_str(), payloadStr.c_str());
}

bool MqttBackend::isReady() {
  return getIsWifiConnected();
}

void MqttBackend::shutdown() {
  wifiStop();
}
#endif 