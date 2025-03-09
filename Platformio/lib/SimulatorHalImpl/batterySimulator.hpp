#include <chrono>
#include <cmath>

#include "Hardware/BatteryInterface.h"
class BatterySimulator : public BatteryInterface {
 public:
  BatterySimulator()
      : mCreationTime(std::chrono::high_resolution_clock::now()) {};
  ~BatterySimulator() {}

  virtual int getPercentage() override {
    auto now = std::chrono::high_resolution_clock::now();
    auto batteryRunTime =
        std::chrono::duration_cast<std::chrono::seconds>(now - mCreationTime);
    constexpr auto minToBatteryZero = 3;
    auto fakeBattPercentage =
        100 - ((batteryRunTime / std::chrono::duration<float, std::ratio<60LL>>(
                                     minToBatteryZero)) *
               100);
    return std::floor(fakeBattPercentage < 100 ? fakeBattPercentage : 0);
  }

  virtual bool isCharging() override { return false; }

 private:
  std::chrono::_V2::system_clock::time_point mCreationTime;
};