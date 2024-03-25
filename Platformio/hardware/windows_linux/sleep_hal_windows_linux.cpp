#include <stdio.h>
#include <stdint.h>

// is "lift to wake" enabled
bool wakeupByIMUEnabled = true;
// timeout before going to sleep
uint32_t sleepTimeout;

void init_sleep_HAL() {}
void init_IMU_HAL(void) {}
void check_activity_HAL() {}
void setLastActivityTimestamp_HAL() {}

uint32_t get_sleepTimeout_HAL() {
  return sleepTimeout;
}
void set_sleepTimeout_HAL(uint32_t aSleepTimeout) {
  sleepTimeout = aSleepTimeout;
  printf("sleep timeout set to %u ms\r\n", aSleepTimeout);
}
bool get_wakeupByIMUEnabled_HAL() {
  return wakeupByIMUEnabled;
}
void set_wakeupByIMUEnabled_HAL(bool aWakeupByIMUEnabled) {
  wakeupByIMUEnabled = aWakeupByIMUEnabled;
  printf("lift to wake set to %d\r\n", aWakeupByIMUEnabled);
}
