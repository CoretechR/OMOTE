#include "EspStats.hpp"
#include "esp_system.h"

EspStats::EspStats() {
  xTaskCreate(&EspStats::NotifyFreeHeapTask, "Free Heap Notifier", 2048, this,
              5, &mPeriodicHeapNotifier);
}

EspStats::~EspStats() { vTaskDelete(mPeriodicHeapNotifier); };

uint32_t EspStats::GetFreeHeap() { return esp_get_free_heap_size(); }

void EspStats::NotifyFreeHeapTask(void *aSelf) {
  auto self = reinterpret_cast<EspStats *>(aSelf);
  while (true) {
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    self->SendFreeHeapNotification();
  }
}
