#include "HardwareAbstract.hpp"

HardwareAbstract::HardwareAbstract()
{}


std::optional<HardwareAbstract::batteryStatus> HardwareAbstract::getBatteryStatus(){
#if 0
    if(mBattery){
        HardwareAbstract::batteryStatus currentStatus;
        currentStatus.percentage = mBattery->getPercentage();
        currentStatus.isCharging = mBattery->isCharging();
        return currentStatus;
    }
#endif
    return std::nullopt;
}

#if 0
void HardwareAbstract::onBatteryChange(std::function<void(HardwareAbstract::batteryStatus)> onBatteryStatusChangeHandler){
  mBatteryNotification.onNotify(std::move(onBatteryStatusChangeHandler));
}
#endif
