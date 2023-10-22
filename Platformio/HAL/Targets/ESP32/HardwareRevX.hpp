#pragma once
#include "SparkFunLIS3DH.h"

#include "HardwareAbstract.hpp"
#include "battery.hpp"
#include "lvgl.h"
#include "wifihandler.hpp"
#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
#include <Preferences.h>
#include <PubSubClient.h>
#include <functional>
#include <memory>

#include "omoteconfig.h"

#include "BatteryInterface.h"
#include "display.hpp"
#include "keys.hpp"
#include "wifiHandlerInterface.h"

class HardwareRevX : public HardwareAbstract {
public:
  enum class WakeReason { RESET, IMU, KEYPAD };

  HardwareRevX();

  // HardwareAbstract
  virtual void init() override;
  virtual void debugPrint(const char *fmt, ...) override;

  virtual std::shared_ptr<BatteryInterface> battery() override;
  virtual std::shared_ptr<DisplayAbstract> display() override;
  virtual std::shared_ptr<wifiHandlerInterface> wifi() override;
  virtual std::shared_ptr<KeyPressAbstract> keys() override;

  virtual char getCurrentDevice() override;
  virtual void setCurrentDevice(char currentDevice) override;

  virtual bool getWakeupByIMUEnabled() override;
  virtual void setWakeupByIMUEnabled(bool wakeupByIMUEnabled) override;

  virtual uint16_t getSleepTimeout() override;
  virtual void setSleepTimeout(uint16_t sleepTimeout) override;

  /// @brief To be ran in loop out in main
  // TODO move to a freertos task
  void loopHandler() override;

protected:
  // Init Functions to setup hardware
  void initIO();
  void restorePreferences();
  void slowDisplayWakeup();
  void setupIMU();
  void setupIR();

  void activityDetection();
  void enterSleep();
  void configIMUInterrupts();

  // Tasks
  void startTasks();

private:
  std::shared_ptr<Battery> mBattery;
  std::shared_ptr<Display> mDisplay;
  std::shared_ptr<wifiHandler> mWifiHandler;
  std::shared_ptr<Keys> mKeys;
  // IMU Motion Detection
  LIS3DH IMU = LIS3DH(I2C_MODE, 0x19); // Default constructor is I2C, addr 0x19.
  int standbyTimer = SLEEP_TIMEOUT;
  int sleepTimeout = SLEEP_TIMEOUT;
  int motion = 0;
  WakeReason wakeup_reason;

  Preferences preferences;
  bool wakeupByIMUEnabled = true;
  byte currentDevice = 1; // Current Device to control (allows switching
                          // mappings between devices)

  // IR declarations
  IRsend IrSender = IRsend(IR_LED, true);
  IRrecv IrReceiver = IRrecv(IR_RX);

  static std::shared_ptr<HardwareRevX> mInstance;
  Handler<TS_Point> mTouchHandler;
};