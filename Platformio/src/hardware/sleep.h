#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <Arduino.h>
#include "SparkFunLIS3DH.h"

#define ACC_INT 20

// IMU declarations
#define DEFAULT_SLEEP_TIMEOUT 20000 // time until device enters sleep mode in milliseconds
extern uint32_t actualSleepTimeout;
extern uint32_t standbyTimer;
extern bool wakeupByIMUEnabled;
#define MOTION_THRESHOLD 50 // motion above threshold keeps device awake

// Other declarations
extern byte wakeup_reason;
enum Wakeup_reasons{WAKEUP_BY_RESET, WAKEUP_BY_IMU, WAKEUP_BY_KEYPAD};

void init_sleep();
void setup_IMU();
void check_activity();
void resetStandbyTimer();

#endif /*__SLEEP_H__*/
