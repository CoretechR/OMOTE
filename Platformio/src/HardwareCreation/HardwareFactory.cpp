#include "HardwareFactory.hpp"

#if defined(IS_SIMULATOR)
#include "HardwareSimulator.hpp"
#else
#include "HardwareRevX.hpp"
#endif

std::unique_ptr<HardwareAbstract> HardwareFactory::mHardware = nullptr;

void HardwareFactory::Init() {
#if defined(IS_SIMULATOR)
  mHardware = std::make_unique<HardwareSimulator>();
#else
  mHardware = std::make_unique<HardwareRevX>();
#endif
  mHardware->init();
}

HardwareAbstract &HardwareFactory::getAbstract() { return *mHardware; }