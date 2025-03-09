#include "HardwareFactory.hpp"

#if OMOTE_SIM
#include "HardwareSimulator.hpp"
#endif

#if OMOTE_ESP32
#include "HardwareRevX.hpp"
#endif

std::unique_ptr<HardwareAbstract> HardwareFactory::mHardware = nullptr;

void HardwareFactory::Init() {
#if OMOTE_SIM
  mHardware = std::make_unique<HardwareSimulator>();
#endif
#if OMOTE_ESP32
  mHardware = std::make_unique<HardwareRevX>();
#endif
  mHardware->init();
}

HardwareAbstract &HardwareFactory::getAbstract() { return *mHardware; }