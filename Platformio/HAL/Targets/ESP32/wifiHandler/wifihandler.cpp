#include "wifihandler.hpp"
#include "HardwareAbstract.hpp"
#include "WiFi.h"
#include <Arduino.h>
#include <Preferences.h>

std::shared_ptr<wifiHandler> wifiHandler::mInstance = nullptr;

// WiFi status event
void wifiHandler::WiFiEvent(WiFiEvent_t event) {
  int no_networks = 0;
  switch (event) {
  case ARDUINO_EVENT_WIFI_SCAN_DONE: {
    Serial.println("WIFI scan done\n");
    no_networks = WiFi.scanComplete();
    Serial.println("making notify vector");
    auto info = std::vector<WifiInfo>(no_networks);
    Serial.println("loaing notify vector");
    for (int i = 0; i < no_networks; i++) {
      auto ssid =
          WiFi.SSID(i).c_str() ? std::string(WiFi.SSID(i).c_str()) : "No SSID";
      info[i] = WifiInfo(ssid, WiFi.RSSI(i));
    }
    if (no_networks < 0) {
      Serial.println("Scan failed");
    } else {
      // TODO Convert To callbacks
      // this->display.clear_wifi_networks();
      Serial.print(no_networks);
      Serial.print(" found\n");
      // this->display.wifi_scan_complete( no_networks);
    }
    Serial.println("notifying");
    mScanNotification->notify(info);
    Serial.println("notified");
    if (WiFi.isConnected() == false) {
      WiFi.reconnect();
    }
    break;
  }
  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
  case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
    this->update_credentials();
  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
  case ARDUINO_EVENT_WIFI_STA_LOST_IP:
  case ARDUINO_EVENT_WIFI_STA_STOP:
    this->update_status();
  default:
    Serial.print("Wifi Status: ");
    Serial.println(WiFi.status());
    break;
  }
  if (WiFi.status() == WL_CONNECT_FAILED) {
    Serial.println("connection failed.");
    WiFi.disconnect();
  }
  Serial.println(WiFi.status());
}

bool wifiHandler::isAvailable() { return true; }
std::shared_ptr<wifiHandler> wifiHandler::getInstance() {
  if (mInstance) {
    return mInstance;
  }
  mInstance = std::shared_ptr<wifiHandler>(new wifiHandler());
  return mInstance;
};

wifiHandler::wifiHandler() {
  this->password = "";
  this->SSID = "";
}

void wifiHandler::update_status() {
  Serial.println("update_status");
  auto isConnected = WiFi.isConnected();
  std::string ip(WiFi.localIP().toString().c_str());
  std::string ssid(WiFi.SSID().c_str());

  wifiStatus status = wifiStatus(isConnected, ip, ssid);

  // Serial.println(WiFi.localIP());
  mStatusUpdate->notify(status);
}

void wifiHandler::update_credentials() {
  // No connection was attempted so don't try to to save the creds
  if (!connect_attempt) {
    return;
  }

  if (temporary_password == password || temporary_ssid == SSID) {
    password = temporary_password;
    SSID = temporary_ssid;

    Preferences preferences;
    preferences.begin("wifiSettings", false);
    String tempString = temporary_password.c_str();
    preferences.putString("password", tempString);
    tempString = temporary_ssid.c_str();
    preferences.putString("SSID", tempString);
    preferences.end();
  }
  this->connect_attempt = false;
}

void wifiHandler::scan() {
  Serial.println("scan called");

  WiFi.scanNetworks(true);
}

void wifiHandler::begin() {
  // this->display = display;
  WiFi.setHostname("OMOTE");
  WiFi.mode(WIFI_STA);
  // WiFi.onEvent([this] (WiFiEvent_t event) {mInstance->WiFiEvent(event);});
  WiFi.onEvent([](WiFiEvent_t event) { mInstance->WiFiEvent(event); });

  Preferences preferences;
  preferences.begin("wifiSettings", false);
  String ssid = preferences.getString("SSID");
  String password = preferences.getString("password");
  preferences.end();

  /* If the SSID is not empty, there was a value stored in the preferences and
   * we try to use it.*/
  if (!ssid.isEmpty()) {
    Serial.print("Connecting to wifi ");
    Serial.println(ssid);
    this->SSID = ssid.c_str();
    this->password = password.c_str();
    this->connect(SSID, this->password);
  } else {
    Serial.println("no SSID or password stored");
    /*Set first character to \0 indicates an empty string*/
    this->SSID[0] = '\0';
    this->password[0] = '\0';
    WiFi.disconnect();
  }

  WiFi.setSleep(true);
}

void wifiHandler::connect(std::string ssid, std::string password) {
  this->connect_attempt = true;
  // this->temporary_password = password;
  // this->temporary_ssid = ssid;
  WiFi.begin(ssid.c_str(), password.c_str());
}

void wifiHandler::turnOff() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void wifiHandler::disconnect() { WiFi.disconnect(); }

bool wifiHandler::isConnected() { return WiFi.isConnected(); }

std::string wifiHandler::getIP() {
  return std::string(WiFi.localIP().toString().c_str());
}