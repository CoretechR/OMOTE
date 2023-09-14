#pragma once
#include <string>
#include <memory>
#include <functional>

typedef struct {
  std::string ssid;
  int rssi;
} WifiInfo;

typedef struct {
  bool isConnected;
  std::string IP;
  std::string ssid;
}wifiStatus;

class wifiHandlerInterface{
    public:
        virtual bool isAvailable() = 0;
        virtual void scan() = 0;
        virtual void connect(std::shared_ptr<std::string> ssid, std::shared_ptr<std::string> password) = 0;
        virtual void onScanDone(std::function<void (std::shared_ptr<std::vector<WifiInfo>>)> function) = 0;
        virtual void onStatusUpdate(std::function<void (std::shared_ptr<wifiStatus>)> function) = 0;
        virtual void begin() = 0;
};