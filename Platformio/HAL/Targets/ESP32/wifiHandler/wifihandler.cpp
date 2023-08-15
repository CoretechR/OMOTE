#include "wifihandler.hpp"
#include <Arduino.h>
#include <Preferences.h>

std::shared_ptr<wifiHandler> wifiHandler::mInstance = nullptr;


// WiFi status event
void wifiHandler::WiFiEvent(WiFiEvent_t event){
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
        // TODO Convert To callbacks
        //this->display.clear_wifi_networks();
        Serial.print(no_networks);
        Serial.print(" found\n");
        //this->display.wifi_scan_complete( no_networks);
      }
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      // TODO convert to callbacks 
      //display.update_wifi(true);
      //update_credentials(temporary_ssid, temporary_password);
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
    case ARDUINO_EVENT_WIFI_STA_STOP:
      // TODO Convert to Callbacks 
      //display.update_wifi(false);
    default:
      break;
  }
}

std::shared_ptr<wifiHandler> wifiHandler::getInstance()
{
    if(mInstance)
    {
      return mInstance;
    }
    return std::shared_ptr<wifiHandler>(new wifiHandler());
};

wifiHandler::wifiHandler()
{
    this->password[0] = '\0';
    this->SSID[0] = '\0';
}

void wifiHandler::update_credentials(const char* temporary_ssid, const char* temporary_password)
{
    if (strcmp(temporary_password, wifiHandler::password) != 0 || strcmp(temporary_ssid, wifiHandler::SSID) != 0)
    {
        strcpy(wifiHandler::password, temporary_password);
        strcpy(wifiHandler::SSID, temporary_ssid);

        Preferences preferences;
        preferences.begin("wifiSettings", false);
        String tempString = wifiHandler::password;
        preferences.putString("password", tempString);
        tempString = wifiHandler::SSID;
        preferences.putString("SSID", tempString);
        preferences.end();
    }
}

void wifiHandler::scan()
{
    WiFi.scanNetworks(true);
}

void wifiHandler::begin()
{
    //this->display = display;
    WiFi.setHostname("OMOTE");
    WiFi.mode(WIFI_STA);
    WiFi.onEvent([] (WiFiEvent_t event) {mInstance->WiFiEvent(event);});

    Preferences preferences;
    preferences.begin("wifiSettings",false);
    String ssid = preferences.getString("SSID");
    String password = preferences.getString("password");
    preferences.end();

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
    strncpy(this->temporary_password, password, STRING_SIZE);
    strncpy(this->temporary_ssid, SSID, STRING_SIZE);
    WiFi.begin(SSID, password);
}

void wifiHandler::turnOff()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void wifiHandler::disconnect(){
  WiFi.disconnect();
}

bool wifiHandler::isConnected()
{
    return WiFi.isConnected();
}

char* wifiHandler::getSSID()
{
    return this->SSID;
}

std::string wifiHandler::getIP()
{
    return std::string(WiFi.localIP().toString().c_str());
}