#pragma once
#include <functional>
#include <memory>
#include <string>

class wifiHandlerInterface {
public:
  wifiHandlerInterface() = default;
  struct WifiInfo {
    WifiInfo() = default;
    WifiInfo(std::string aSsid, int aRssi) : ssid(aSsid), rssi(aRssi) {}

    std::string ssid = "";
    int rssi = 0;
  };

  struct wifiStatus {
    wifiStatus() = default;
    wifiStatus(bool aConnected, std::string aIp, std::string aSsid)
        : isConnected(aConnected), IP(aIp), ssid(aSsid){};

    bool isConnected = false;
    std::string IP = "";
    std::string ssid = "";
  };

  typedef std::vector<WifiInfo> ScanDoneDataTy;
  typedef Notification<ScanDoneDataTy> ScanNotificationTy;

  /// @brief Initialize the wifi handler
  virtual void begin() = 0;
  /// @brief Trigger a scan scan for wifi networks
  virtual void scan() = 0;
  /// @brief Attempt a connection to the wifi using the provided credentials
  virtual void connect(std::string ssid, std::string password) = 0;
  /// @brief Get the status of the current wifi connection
  virtual wifiStatus GetStatus() = 0;

  // Register for Scan Notification to handle when scans are completed
  std::shared_ptr<ScanNotificationTy> ScanCompleteNotification() {
    return mScanNotification;
  };

  // Register for Status notifications to handle changes in status
  std::shared_ptr<Notification<wifiStatus>> WifiStatusNotification() {
    return mStatusUpdate;
  };

protected:
  std::shared_ptr<ScanNotificationTy> mScanNotification =
      std::make_shared<ScanNotificationTy>();
  std::shared_ptr<Notification<wifiStatus>> mStatusUpdate =
      std::make_shared<Notification<wifiStatus>>();
};