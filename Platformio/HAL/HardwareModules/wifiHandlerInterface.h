#pragma once
#include <functional>
#include <memory>
#include <string>

class wifiHandlerInterface {
public:
  struct WifiInfo {
    WifiInfo(){};
    WifiInfo(std::string aSsid, int aRssi) : ssid(aSsid), rssi(aRssi) {}

    std::string ssid = "";
    int rssi = 0;
  };

  struct wifiStatus {
    wifiStatus(bool aConnected, std::string aIp, std::string aSsid)
        : isConnected(aConnected), IP(aIp), ssid(aSsid){};

    bool isConnected;
    std::string IP = "";
    std::string ssid = "";
  };

  typedef std::vector<WifiInfo> ScanDoneDataTy;
  typedef Notification<ScanDoneDataTy> ScanNotificationTy;

  virtual void begin() = 0;
  virtual void scan() = 0;
  virtual void connect(std::string ssid, std::string password) = 0;
  virtual bool isAvailable() = 0;

  std::shared_ptr<ScanNotificationTy> ScanCompleteNotification() {
    return mScanNotification;
  };

  std::shared_ptr<Notification<wifiStatus>> WifiStatusNotification() {
    return mStatusUpdate;
  };

protected:
  std::shared_ptr<ScanNotificationTy> mScanNotification =
      std::make_shared<ScanNotificationTy>();
  std::shared_ptr<Notification<wifiStatus>> mStatusUpdate =
      std::make_shared<Notification<wifiStatus>>();
};