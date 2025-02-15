#include "HardwareSimulator.hpp"

#include <sstream>

#include "SDLDisplay.hpp"

HardwareSimulator::HardwareSimulator()
    : HardwareAbstract(),
      mBattery(std::make_shared<BatterySimulator>()),
      mDisplay(SDLDisplay::getInstance()),
      mWifiHandler(std::make_shared<wifiHandlerSim>()),
      mKeys(std::make_shared<KeyPressSim>()),
      mIr(std::make_shared<IRSim>()),
      mStats(std::make_shared<StatsSimulator>()) {
  mHardwareStatusTitleUpdate = std::thread([this] {
    int dataToShow = 0;
    while (true) {
      std::stringstream title;
      switch (dataToShow) {
        case 0:
          title << "Batt:" << mBattery->getPercentage() << "%" << std::endl;
          break;
        case 1:
          title << "BKLght: " << static_cast<int>(mDisplay->getBrightness())
                << std::endl;
          dataToShow = -1;
          break;
        default:
          dataToShow = -1;
      }
      dataToShow++;

      mDisplay->setTitle(title.str());
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  });
}

std::shared_ptr<BatteryInterface> HardwareSimulator::battery() {
  return mBattery;
}
std::shared_ptr<DisplayAbstract> HardwareSimulator::display() {
  return mDisplay;
}
std::shared_ptr<wifiHandlerInterface> HardwareSimulator::wifi() {
  return mWifiHandler;
}
std::shared_ptr<KeyPressAbstract> HardwareSimulator::keys() { return mKeys; }

std::shared_ptr<IRInterface> HardwareSimulator::ir() { return mIr; }

std::shared_ptr<SystemStatsInterface> HardwareSimulator::stats() {
  return mStats;
}

char HardwareSimulator::getCurrentDevice() { return 0; }

void HardwareSimulator::setCurrentDevice(char currentDevice) {}

bool HardwareSimulator::getWakeupByIMUEnabled() { return true; }

void HardwareSimulator::setWakeupByIMUEnabled(bool wakeupByIMUEnabled) {}

uint16_t HardwareSimulator::getSleepTimeout() { return 20000; }

void HardwareSimulator::setSleepTimeout(uint16_t sleepTimeout) {}
