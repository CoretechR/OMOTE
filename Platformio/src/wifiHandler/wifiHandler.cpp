/**
 * @file wifiHandler.cpp
 * @author Thomas Bittner
 * @brief Wifi handler is a wrapper for the arduino wifi function. This wrapper provides handling of wifi credetials and
 * storing it over reset.
 * @version 0.1
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "wifiHandler.hpp"
#include <Preferences.h>
#include <WiFi.h>
#include "display.hpp"
#include <PubSubClient.h>

/*TODO: rework to not use global variables*/
extern wifiHandler wifihandler;
extern Preferences preferences;
extern Display display;
extern PubSubClient client;
const char* temporary_ssid;
const char* temporary_password;

#define MQTT_SERVER "YOUR_MQTT_SERVER_IP"
// WiFi status event
void WiFiEvent(WiFiEvent_t event){
  int no_networks = 0;
  switch (event)
  {
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
      Serial.println("WIFI scan done\n");
      no_networks = WiFi.scanComplete();
      if (no_networks < 0)
      {
        Serial.println("Scan failed");
      }
      else
      {
        display.clear_wifi_networks();
        Serial.print(no_networks);
        Serial.print(" found\n");
        String SSIDS[no_networks];
        int RSSIS[no_networks];
        for (int i = 0; i < no_networks; i++)
        {
          SSIDS[i] = WiFi.SSID(i);
          RSSIS[i] = WiFi.RSSI(i);
        }
        display.add_wifi_networks(SSIDS, RSSIS, no_networks);
      }
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      client.setServer(MQTT_SERVER, 1883); // MQTT initialization
      client.connect("OMOTE"); // Connect using a client id
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      display.update_wifi(true);
      wifihandler.update_credetials(temporary_ssid, temporary_password);
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
    case ARDUINO_EVENT_WIFI_STA_STOP:
      display.update_wifi(false);
    default:
      break;
  }
}
wifiHandler::wifiHandler()
{
    this->password[0] = '\0';
    this->SSID[0] = '\0';
}


void wifiHandler::update_credetials(const char* temporary_ssid, const char* temporary_password)
{
    if (strcmp(temporary_password, this->password) != 0 || strcmp(temporary_ssid, this->SSID) != 0)
    {
        strcpy(this->password, temporary_password);
        strcpy(this->SSID, temporary_ssid);

        String tempString = this->password;
        preferences.putString("password", tempString);
        tempString = this->SSID;
        preferences.putString("SSID", tempString);
    }
}

void wifiHandler::scan()
{
    WiFi.scanNetworks(true);
}

void wifiHandler::begin()
{
    WiFi.setHostname("OMOTE");
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(WiFiEvent);

    String ssid = preferences.getString("SSID");
    String password = preferences.getString("password");

    /* If the SSID is not empty, there was a value stored in the preferences and we try to use it.*/
    if (!ssid.isEmpty())
    {
        Serial.print("Connecting to wifi ");
        Serial.println(ssid);
        strcpy(this->SSID,  ssid.c_str());
        strcpy(this->password, password.c_str());
        this->connect(this->SSID, this->password);
    }
    else
    {
        Serial.println("no SSID or password stored");
        /*Set first character to \0 indicates an empty string*/
        this->SSID[0] = '\0';
        this->password[0] = '\0';
        WiFi.disconnect();
    }

    WiFi.setSleep(true);
}

void wifiHandler::connect(const char* SSID, const char* password)
{
    temporary_password = password;
    temporary_ssid = SSID;
    WiFi.begin(SSID, password);
}

void wifiHandler::turnOff()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

bool wifiHandler::isConnected()
{
    return WiFi.isConnected();
}

char* wifiHandler::getSSID()
{
    return this->SSID;
}

String wifiHandler::getIP()
{
    return WiFi.localIP().toString();
}