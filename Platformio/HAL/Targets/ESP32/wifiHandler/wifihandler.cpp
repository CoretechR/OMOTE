#include "wifihandler.hpp"
#include "HardwareAbstract.hpp"
#include "WiFi.h"
#include <Arduino.h>
#include <Preferences.h>

#include "omoteconfig.h"

std::shared_ptr<wifiHandler> wifiHandler::mInstance = nullptr;
std::shared_ptr<wifiHandler> wifiHandler::getInstance() {
  if (mInstance) {
    return mInstance;
  }
  mInstance = std::shared_ptr<wifiHandler>(new wifiHandler());
  return mInstance;
};

void wifiHandler::WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t aEventInfo) {
  int no_networks = 0;
  switch (event) {
  case ARDUINO_EVENT_WIFI_SCAN_DONE: {
    no_networks = WiFi.scanComplete();
    auto info = std::vector<WifiInfo>(no_networks);
    for (int i = 0; i < no_networks; i++) {
      auto ssid =
          WiFi.SSID(i).c_str() ? std::string(WiFi.SSID(i).c_str()) : "No SSID";
      info[i] = WifiInfo(ssid, WiFi.RSSI(i));
    }
    mScanNotification->notify(info);
    if (WiFi.isConnected() == false) {
      WiFi.reconnect();
    }
    break;
  }
  case ARDUINO_EVENT_WIFI_STA_CONNECTED:
    StoreCredentials();
    WiFi.setAutoConnect(true);
    UpdateStatus();
    break;
  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
  case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
  case ARDUINO_EVENT_WIFI_STA_LOST_IP:
  case ARDUINO_EVENT_WIFI_STA_STOP:
    UpdateStatus();
    break;
  default:
    break;
  }
  if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("connection failed.");
    WiFi.disconnect();
  }
  Serial.println(WiFi.status());
}

void wifiHandler::UpdateStatus() {
  Serial.println("update_status");

  wifiStatus status;
  // wifiStatus *status = new wifiStatus();
  status.isConnected = WiFi.isConnected();
  // status->IP = WiFi.localIP();
  IPAddress ip = WiFi.localIP();
  String ip_str = ip.toString();
  status.IP = ip.toString().c_str();

  // ip.copy(status->IP, ip.length());
  String ssid = WiFi.SSID();
  status.ssid = WiFi.SSID().c_str();

  // this->wifi_status.isConnected = WiFi.isConnected();
  // this->wifi_status.IP = WiFi.localIP();
  // this->wifi_status.isConnected = true;

  // Serial.println(WiFi.localIP());
  mStatusUpdate->notify(status);
}

void wifiHandler::StoreCredentials() {
  // No connection was attempted so don't try to to save the creds
  if (!mIsConnectionAttempt) {
    return;
  }
  mPassword = mConnectionAttemptPassword;
  mSSID = mConnectionAttemptSSID;

  Preferences preferences;
  preferences.begin("wifiSettings", false);
  preferences.putString("password", mPassword.c_str());
  preferences.putString("SSID", mSSID.c_str());
  preferences.end();

  mConnectionAttemptPassword.clear();
  mConnectionAttemptSSID.clear();
  mIsConnectionAttempt = false;
}

void wifiHandler::scan() {
  Serial.println("scan called");
  WiFi.setAutoReconnect(false);
  WiFi.scanNetworks(true);
}

void wifiHandler::begin() {
  WiFi.setHostname("OMOTE");
  WiFi.mode(WIFI_STA);
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t aEventInfo) {
    mInstance->WiFiEvent(event, aEventInfo);
  });

  Preferences preferences;
  preferences.begin("wifiSettings", false);
  String ssid = preferences.getString("SSID");
  String password = preferences.getString("password");
  preferences.end();

  // Attempt Connection with stored Credentials
  if (!ssid.isEmpty()) {
    connect(ssid.c_str(), password.c_str());
  } else {
    Serial.println("no SSID or password stored");
    WiFi.disconnect();
  }

  WiFi.setSleep(true);
}

void wifiHandler::connect(std::string ssid, std::string password) {
  Serial.printf("Attempting Wifi Connection To %s \n", ssid.c_str());
  mIsConnectionAttempt = true;
  mConnectionAttemptPassword = password;
  mConnectionAttemptSSID = ssid;
  auto status = WiFi.begin(mConnectionAttemptSSID.c_str(),
                           mConnectionAttemptPassword.c_str());
}

void wifiHandler::mqttSend(std::string aTopic, std::string aMessage) {
  if (mMqttClient.connected() || mMqttClient.connect(MQTT_CLIENT_NAME)) {
    mMqttClient.publish(aTopic.c_str(), aMessage.c_str());
    return;
  } else {
    Serial.println("Failed to Send MQTT due to Connection Failure");
  }
}

void wifiHandler::setupMqttBroker(std::string aBrokerIpAddress, int aPort) {
  mMqttClient.setServer(aBrokerIpAddress.c_str(), aPort);
  if (!mMqttClient.connect(MQTT_CLIENT_NAME)) {
    Serial.print("Failed to Connect to MQTT Server at:");
    Serial.print(aBrokerIpAddress.c_str());
    Serial.print("  Port:");
    Serial.println(aPort);
  };
}
