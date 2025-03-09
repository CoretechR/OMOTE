#pragma once
#include "Hardware/SystemStatsInterface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class EspStats : public SystemStatsInterface {
public:
  EspStats();
  ~EspStats() override;

  uint32_t GetFreeHeap() override;

private:
  static void NotifyFreeHeapTask(void *);

  TaskHandle_t mPeriodicHeapNotifier = nullptr;
};
