#pragma once
#include <random>
#include <thread>

#include "Hardware/SystemStatsInterface.h"

class StatsSimulator : public SystemStatsInterface {
 public:
  StatsSimulator();
  ~StatsSimulator() override = default;

  uint32_t GetFreeHeap() override;

 private:
  std::thread mHeapNotifyThread;
};

inline StatsSimulator::StatsSimulator()
// : mHeapNotifyThread([this] {
//     while (true) {
//       SendFreeHeapNotification();
//       std::this_thread::sleep_for(std::chrono::seconds(2));
//     }
//   })

{}

inline uint32_t StatsSimulator::GetFreeHeap() { return std::rand() % 90000; }
