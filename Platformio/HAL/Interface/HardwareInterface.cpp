#include "HardwareInterface.h"

HardwareInterface::HardwareInterface(std::shared_ptr<BatteryInterface> aBattery)
: mBattery(std::move(aBattery)){

}

std::optional<HardwareInterface::batteryStatus> HardwareInterface::getBatteryStatus(){
    if(mBattery){
        HardwareInterface::batteryStatus currentStatus;
        currentStatus.percentage = mBattery->getPercentage();
        currentStatus.isCharging = mBattery->isCharging();
        return currentStatus;
    }
    return std::nullopt;
}