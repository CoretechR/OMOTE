#pragma once
#include "SparkFunLIS3DH.h"

#include "HardwareAbstract.hpp"
#include "lvgl.h"
#include "battery.hpp"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
#include <Keypad.h> // modified for inverted logic
#include <Preferences.h>
#include <PubSubClient.h>
#include <functional>
#include <memory>
#include "wifihandler.hpp"


#include "omoteconfig.h"
#include "BatteryInterface.h"
#include "wifiHandlerInterface.h"
#include "DisplayAbstract.h"


class HardwareRevX : public HardwareAbstract {
public:
  enum class WakeReason { RESET, IMU, KEYPAD };

  static std::shared_ptr<HardwareRevX> getInstance();
  static std::weak_ptr<HardwareRevX> getRefrence() { return getInstance(); }

  // HardwareAbstract
  virtual void init() override;
  void debugPrint(const char* fmt, ...);

  void loopHandler();

  std::shared_ptr<wifiHandlerInterface> wifi();

protected:
  // Init Functions to setup hardware
  void initIO();
  void setupBacklight();
  void restorePreferences();
  void slowDisplayWakeup();
  void setupIMU();
  void setupIR();

  void activityDetection();
  void enterSleep();
  void configIMUInterrupts();

  // Tasks
  void startTasks();
  /// @brief Send Battery Notification every 5 Seconds
  static void updateBatteryTask(void *);

  TaskHandle_t batteryUpdateTskHndl = nullptr;

private:
  HardwareRevX();

  std::shared_ptr<BatteryInterface> mBattery;
  std::shared_ptr<wifiHandlerInterface> mWifiHandler;
  std::shared_ptr<DisplayAbstract> mDisplay;
  // IMU Motion Detection
  LIS3DH IMU = LIS3DH(I2C_MODE, 0x19); // Default constructor is I2C, addr 0x19.
  int standbyTimer = SLEEP_TIMEOUT;
  int motion = 0;
  WakeReason wakeup_reason;

  Preferences preferences;
  bool wakeupByIMUEnabled = true;
  int backlight_brightness = 255;
  byte currentDevice = 1; // Current Device to control (allows switching
                          // mappings between devices)

  // IR declarations
  IRsend IrSender = IRsend(IR_LED, true);
  IRrecv IrReceiver = IRrecv(IR_RX);


  // Keypad declarations
  static const byte ROWS = 5; // four rows
  static const byte COLS = 5; // four columns
  // define the symbols on the buttons of the keypads
  char hexaKeys[ROWS][COLS] = {
      {'s', '^', '-', 'm', 'r'}, //  source, channel+, Volume-,   mute, record
      {'i', 'r', '+', 'k', 'd'}, //    info,    right, Volume+,     OK,   down
      {'4', 'v', '1', '3', '2'}, //    blue, channel-,     red, yellow,  green
      {'>', 'o', 'b', 'u', 'l'}, // forward,      off,    back,     up,   left
      {'?', 'p', 'c', '<', '='}  //       ?,     play,  config, rewind,   stop
  };
  byte rowPins[ROWS] = {SW_A, SW_B, SW_C, SW_D,
                        SW_E}; // connect to the row pinouts of the keypad
  byte colPins[COLS] = {SW_1, SW_2, SW_3, SW_4,
                        SW_5}; // connect to the column pinouts of the keypad
  Keypad customKeypad =
      Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

  byte keyMapTechnisat[ROWS][COLS] = {{0x69, 0x20, 0x11, 0x0D, 0x56},
                                      {0x4F, 0x37, 0x10, 0x57, 0x51},
                                      {0x6E, 0x21, 0x6B, 0x6D, 0x6C},
                                      {0x34, 0x0C, 0x22, 0x50, 0x55},
                                      {'?', 0x35, 0x2F, 0x32, 0x36}};
  byte virtualKeyMapTechnisat[10] = {0x1, 0x2, 0x3, 0x4, 0x5,
                                     0x6, 0x7, 0x8, 0x9, 0x0};

  static std::shared_ptr<HardwareRevX> mInstance;
};