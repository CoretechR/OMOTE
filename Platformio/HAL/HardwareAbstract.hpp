// OMOTE Hardware Abstraction
// 2023 Matthew Colvin

#pragma once
#include <functional>
#include <lvgl.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "BatteryInterface.h"
#include "DisplayAbstract.h"
#include "wifiHandlerInterface.h"
#include "Notification.hpp"

typedef struct {
  std::string ssid;
  int rssi;
} WifiInfo;

class HardwareAbstract {
public:
  HardwareAbstract(
    std::shared_ptr<DisplayAbstract> aDisplay,
    std::shared_ptr<BatteryInterface> aBattery = nullptr,
    std::shared_ptr<wifiHandlerInterface> aWifiHandler = nullptr
  );

  struct batteryStatus {
      /// @brief Percent of battery remaining (0-100]
      int percentage;
      /// @brief True - Battery is Charging
      ///        False - Battery discharging
      bool isCharging;
  };
  virtual std::optional<batteryStatus> getBatteryStatus();

  /// @brief Register function to be ran when hardware notifies battery
  ///        status has changed.
  /// @param onBatteryStatusChangeHandler - Callable to be ran when batter status changes
  void onBatteryChange(std::function<void(batteryStatus)> onBatteryStatusChangeHandler);

  void onStartWifiScan(std::function<void()> cb_func);
  void onWifiScanDone(std::function<void(std::shared_ptr<std::vector<WifiInfo>>)> cb_func);
  void notifyStartWifiScan();
  void notifyWifiScanDone(std::shared_ptr<std::vector<WifiInfo>> info);

  /// @brief Override in order to do setup of hardware devices
  virtual void init() = 0;

  /// @brief Override to allow printing of a message for debugging
  /// @param message - Debug message
  virtual void debugPrint(std::string message) = 0;

  protected:
    Notification<batteryStatus> mBatteryNotification;

  private:
    std::vector<std::function<void()>> wifi_scan_start_cb;
    std::vector<std::function<void(std::shared_ptr<std::vector<WifiInfo>>)>> wifi_scan_done_cb;
    std::shared_ptr<BatteryInterface> mBattery;
    std::shared_ptr<wifiHandlerInterface> mWifiHandler;
    std::shared_ptr<DisplayAbstract> mDisplay;

};
