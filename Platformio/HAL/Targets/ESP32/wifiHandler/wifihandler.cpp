#include "wifihandler.hpp"
#include <Arduino.h>
#include <Preferences.h>
#include "HardwareAbstract.hpp"

std::shared_ptr<wifiHandler> wifiHandler::mInstance = nullptr;

// WiFi status event
void wifiHandler::WiFiEvent(WiFiEvent_t event){
  int no_networks = 0;
  switch (event)
  {
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
    {
      Serial.println("WIFI scan done\n");
      no_networks = WiFi.scanComplete();
      std::vector<WifiInfo> *vec = new std::vector<WifiInfo>();
      std::shared_ptr<std::vector<WifiInfo>> info = std::shared_ptr<std::vector<WifiInfo>>(vec);

      for (int i = 0; i < no_networks; i++)
      {
        info->push_back(WifiInfo {
          .ssid = std::string(WiFi.SSID(i).c_str()),
          .rssi = WiFi.RSSI(i)
        });
      }
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
      this->scan_notification.notify(info);
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
      break;
  }
  if (WiFi.status() == WL_CONNECT_FAILED)
  {
    Serial.println("connection failed.");
    WiFi.disconnect();
  }
  Serial.println(WiFi.status());
}

bool wifiHandler::isAvailable(){
  return true;
}
std::shared_ptr<wifiHandler> wifiHandler::getInstance()
{
    if(mInstance)
    {
      return mInstance;
    }
    mInstance = std::shared_ptr<wifiHandler>(new wifiHandler());
    return mInstance;
};

wifiHandler::wifiHandler()
{
    this->password = "";
    this->SSID = "";
    this->begin();
}

void wifiHandler::update_status()
{
  Serial.println("update_status");
  std::shared_ptr<wifiStatus> status = std::make_shared<wifiStatus>(wifiStatus());
  //wifiStatus *status = new wifiStatus();
  status->isConnected = WiFi.isConnected();
  //status->IP = WiFi.localIP();
  IPAddress ip = WiFi.localIP();
  String ip_str = ip.toString();
  status->IP = ip.toString().c_str();
  
  //ip.copy(status->IP, ip.length());
  String ssid = WiFi.SSID();
  status->ssid = WiFi.SSID().c_str();

  //this->wifi_status.isConnected = WiFi.isConnected();
  //this->wifi_status.IP = WiFi.localIP();
  //this->wifi_status.isConnected = true;
  

  //Serial.println(WiFi.localIP());
  this->status_update.notify(status);
}

void wifiHandler::update_credentials()
{
#if 0
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
#else
  if (this->temporary_password->compare(this->password) != 0 || this->temporary_ssid->compare(this->SSID))
  {
    this->password = *(this->temporary_password);
    this->SSID = *(this->temporary_ssid);

    Preferences preferences;
    preferences.begin("wifiSettings", false);
    String tempString = this->temporary_password->c_str();
    preferences.putString("password", tempString);
    tempString = this->temporary_ssid->c_str();
    preferences.putString("SSID", tempString);
    preferences.end();
  }
#endif
}

void wifiHandler::scan()
{
  Serial.println("scan called");
    WiFi.scanNetworks(true);
}


void wifiHandler::begin()
{
    //this->display = display;
    WiFi.setHostname("OMOTE");
    WiFi.mode(WIFI_STA);
    //WiFi.onEvent([this] (WiFiEvent_t event) {mInstance->WiFiEvent(event);});
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
        //strcpy(this->SSID,  ssid.c_str());
        //strcpy(this->password, password.c_str());
        this->SSID = ssid.c_str();
        this->password = password.c_str();
        //this->connect(this->SSID, this->password);
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

void wifiHandler::onScanDone(std::function<void (std::shared_ptr<std::vector<WifiInfo>>)> function){
  this->scan_notification.onNotify(std::move(function));
}

void wifiHandler::onStatusUpdate(std::function<void (std::shared_ptr<wifiStatus>)> function){
  this->status_update.onNotify(std::move(function));
}

void wifiHandler::connect(std::shared_ptr<std::string> ssid, std::shared_ptr<std::string> password)
{
  this->temporary_password = password;
  this->temporary_ssid = ssid;
  WiFi.begin(ssid->c_str(), password->c_str());
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

std::string wifiHandler::getIP()
{
    return std::string(WiFi.localIP().toString().c_str());
}