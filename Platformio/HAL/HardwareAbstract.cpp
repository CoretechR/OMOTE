#include "HardwareAbstract.hpp"

HardwareAbstract::HardwareAbstract(
    std::shared_ptr<DisplayAbstract> aDisplay,
    std::shared_ptr<BatteryInterface> aBattery,
    std::shared_ptr<wifiHandlerInterface> aWifiHandler
)
: mBattery(std::move(aBattery)),
  mWifiHandler(std::move(aWifiHandler)),
  mDisplay(std::move(aDisplay))
{}


std::optional<HardwareAbstract::batteryStatus> HardwareAbstract::getBatteryStatus(){
    if(mBattery){
        HardwareAbstract::batteryStatus currentStatus;
        currentStatus.percentage = mBattery->getPercentage();
        currentStatus.isCharging = mBattery->isCharging();
        return currentStatus;
    }
    return std::nullopt;
}

void HardwareAbstract::onBatteryChange(std::function<void(HardwareAbstract::batteryStatus)> onBatteryStatusChangeHandler){
  mBatteryNotification.onNotify(std::move(onBatteryStatusChangeHandler));
}

void HardwareAbstract::onStartWifiScan(std::function<void()> cb_func){
  this->wifi_scan_start_cb.push_back(cb_func);
}

void HardwareAbstract::onWifiScanDone(std::function<void(std::shared_ptr<std::vector<WifiInfo>>)> cb_func){
  this->wifi_scan_done_cb.push_back(cb_func);
}

void HardwareAbstract::notifyStartWifiScan(){
    for (std::function<void()> cb_func:this->wifi_scan_start_cb)
    {
      cb_func();
    }
}

void HardwareAbstract::notifyWifiScanDone(std::shared_ptr<std::vector<WifiInfo>> info){
  for (std::function<void(std::shared_ptr<std::vector<WifiInfo>>)> cb_func: this->wifi_scan_done_cb)
  {
    cb_func(info);
  }
}
