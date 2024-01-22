#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <Arduino.h>
#include "SparkFunLIS3DH.h"

#define ACC_INT 20

// IMU declarations
#define SLEEP_TIMEOUT 20000 // time until device enters sleep mode in milliseconds
#define MOTION_THRESHOLD 50 // motion above threshold keeps device awake
extern int standbyTimer;
extern bool wakeupByIMUEnabled;

// Other declarations
extern byte wakeup_reason;
enum Wakeup_reasons{WAKEUP_BY_RESET, WAKEUP_BY_IMU, WAKEUP_BY_KEYPAD};

void activityDetection();
void enterSleep();

void init_sleep();
void setup_IMU();
void check_activity();

#endif /*__SLEEP_H__*/
