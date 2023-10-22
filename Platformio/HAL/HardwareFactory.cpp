#include "HardwareFactory.hpp"

#if OMOTE_SIM
#include "HardwareSimulator.hpp"
#endif

#if OMOTE_ESP32
#include "HardwareRevX.hpp"
#endif

#if OMOTE_SIM
std::unique_ptr<HardwareAbstract> HardwareFactory::mHardware =
    std::make_unique<HardwareSimulator>();
#endif
#if OMOTE_ESP32
std::unique_ptr<HardwareAbstract> HardwareFactory::mHardware =
    std::make_unique<HardwareRevX>();
#endif

HardwareAbstract &HardwareFactory::getAbstract() { return *mHardware; }