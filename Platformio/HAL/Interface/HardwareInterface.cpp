#include "HardwareInterface.h"

HardwareInterface::HardwareInterface(std::shared_ptr<BatteryInterface> aBattery)
: mBattery(aBattery){
    
}
