#include "wifihandler.hpp"
#include "HardwareAbstract.hpp"
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>

std::shared_ptr<wifiHandler> wifiHandler::mInstance = nullptr;
std::shared_ptr<wifiHandler> wifiHandler::getInstance() {
  if (mInstance) {
    return mInstance;
  }
  mInstance = std::shared_ptr<wifiHandler>(new wifiHandler());
  return mInstance;
};

void wifiHandler::WiFiEvent(WiFiEvent_t event) {
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
  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
  case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
    this->StoreCredentials();
    break;
  case ARDUINO_EVENT_WIFI_STA_CONNECTED:
  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
  case ARDUINO_EVENT_WIFI_STA_LOST_IP:
  case ARDUINO_EVENT_WIFI_STA_STOP:
    UpdateStatus();
    break;
  default:
    break;
  }
  if (WiFi.status() == WL_CONNECT_FAILED) {
    WiFi.disconnect();
  }
}

void wifiHandler::UpdateStatus() {
  Serial.println("UpdateStatus");
  mCurrentStatus.isConnected = WiFi.isConnected();
  mCurrentStatus.IP = WiFi.localIP().toString().c_str();
  mCurrentStatus.ssid = WiFi.SSID().c_str();

  mStatusUpdate->notify(mCurrentStatus);
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

  WiFi.scanNetworks(true);
}

void wifiHandler::begin() {
  WiFi.setHostname("OMOTE");
  WiFi.mode(WIFI_STA);
  WiFi.onEvent([](WiFiEvent_t event) { mInstance->WiFiEvent(event); });

  Preferences preferences;
  preferences.begin("wifiSettings", false);
  String ssid = preferences.getString("SSID");
  String password = preferences.getString("password");
  preferences.end();

  // Attempt Connection with stored Credentials
  if (!ssid.isEmpty()) {
    connect(mSSID, mPassword);
  } else {
    Serial.println("no SSID or password stored");
    WiFi.disconnect();
  }

  WiFi.setSleep(true);
}

void wifiHandler::connect(std::string ssid, std::string password) {
  Serial.printf("Attempting Wifi Connection To %s \n", mSSID.c_str());
  mIsConnectionAttempt = true;
  mConnectionAttemptPassword = password;
  mConnectionAttemptSSID = ssid;
  WiFi.begin(ssid.c_str(), password.c_str());
}
