#include "HardwareAbstract.hpp"

HardwareAbstract::HardwareAbstract(
    std::shared_ptr<BatteryInterface> aBattery,
    std::shared_ptr<wifiHandlerInterface> aWifiHandler,
    std::shared_ptr<DisplayInterface> aDisplay
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

  void HardwareAbstract::notifyBatteryChange(HardwareAbstract::batteryStatus aStatus){
    for (auto handler : mBatteryEventHandlers){
        handler(aStatus);
    }
  }

  void HardwareAbstract::onBatteryChange(std::function<void(HardwareAbstract::batteryStatus)> onBatteryStatusChangeHandler){
    mBatteryEventHandlers.push_back(std::move(onBatteryStatusChangeHandler));
  }