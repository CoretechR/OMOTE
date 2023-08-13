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

void HardwareAbstract::onBatteryChange(std::function<void(HardwareAbstract::batteryStatus)> onBatteryStatusChangeHandler){
  mBatteryNotification.onNotify(std::move(onBatteryStatusChangeHandler));
}