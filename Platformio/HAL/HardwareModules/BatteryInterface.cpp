#include "BatteryInterface.h"

void BatteryInterface::NotifyCurrentStatus(){
    mBatteryNotification.notify(getPercentage(),isCharging());
}

void BatteryInterface::onBatteryStatusChange(std::function<void(int,bool)> batteryChangeHandler){
    mBatteryNotification.onNotify(std::move(batteryChangeHandler));
}