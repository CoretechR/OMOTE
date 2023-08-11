#include "HardwareInterface.h"

HardwareAbstract::HardwareAbstract(std::shared_ptr<BatteryInterface> aBattery)
: mBattery(std::move(aBattery)){

}

std::optional<HardwareAbstract::batteryStatus> HardwareAbstract::getBatteryStatus(){
    if(mBattery){
        HardwareAbstract::batteryStatus currentStatus;
        currentStatus.percentage = mBattery->getPercentage();
        currentStatus.isCharging = mBattery->isCharging();
        return currentStatus;
    }
    return std::nullopt;
}