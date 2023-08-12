#include "HardwareAbstract.hpp"

HardwareAbstract::HardwareAbstract(
    std::shared_ptr<BatteryInterface> aBattery,
    std::shared_ptr<wifiHandlerInterface> aWifiHandler
)
: mBattery(std::move(aBattery)),
  mWifiHandler(std::move(aWifiHandler))
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