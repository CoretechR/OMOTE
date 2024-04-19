#pragma once

void init_sleep_HAL();
void init_IMU_HAL();
void check_activity_HAL();
void setLastActivityTimestamp_HAL();

uint32_t get_sleepTimeout_HAL();
void set_sleepTimeout_HAL(uint32_t aSleepTimeout);
bool get_wakeupByIMUEnabled_HAL();
void set_wakeupByIMUEnabled_HAL(bool aWakeupByIMUEnabled);
char get_wakeupByIMUthreshold_HAL();
void set_wakeupByIMUthreshold_HAL(char awakeupByIMUthreshold);
