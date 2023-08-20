// OMOTE Hardware Abstraction
// 2023 Matthew Colvin
#ifndef _HARDWAREABSTRACT_H_
#define _HARDWAREABSTRACT_H_
#include <functional>
#include <lvgl.h>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "Notification.hpp"

typedef struct {
  std::string ssid;
  int rssi;
} WifiInfo;

typedef struct {
  bool isConnected;
  std::string IP;
  std::string ssid;
}wifiStatus;

class HardwareAbstract {
public:
  HardwareAbstract(
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
  

  /// @brief Override in order to do setup of hardware devices
  virtual void init() = 0;

  /// @brief Override to allow printing of a message for debugging
  /// @param message - Debug message
  virtual void debugPrint(const char* fmt, ...) = 0;

  Notification<std::shared_ptr<std::vector<WifiInfo>>> wifi_scan_done;
  Notification<> wifi_scan_start;
  Notification<std::shared_ptr<std::string>, std::shared_ptr<std::string>> wifi_connect;
  Notification<std::shared_ptr<wifiStatus>> wifi_status_update;

  protected:
    Notification<batteryStatus> mBatteryNotification;

  private:

};
#endif