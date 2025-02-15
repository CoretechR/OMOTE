#pragma once
#include "SystemStatsInterface.h"
#include <random>
#include <thread>

class StatsSimulator : public SystemStatsInterface {
public:
  StatsSimulator()
      : mHeapNotifyThread([this] {
          while (true) {
            SendFreeHeapNotification();
            std::this_thread::sleep_for(std::chrono::seconds(2));
          }
        }){};
  ~StatsSimulator() override = default;

  uint32_t GetFreeHeap() override { return std::rand() % 90000; }

private:
  std::thread mHeapNotifyThread;
};
