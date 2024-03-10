#include <Arduino.h>

// OMOTE firmware for ESP32
// 2023 Maximilian Kern

// hardware
#include "hardware/battery.h"
#include "hardware/sleep.h"
#include "hardware/user_led.h"
#include "hardware/tft.h"
#include "hardware/mqtt.h"
#include "hardware/infrared_sender.h"
#include "hardware/infrared_receiver.h"
// devices
#include "device_samsungTV/device_samsungTV.h"
#include "device_yamahaAmp/device_yamahaAmp.h"
#include "device_smarthome/device_smarthome.h"
#include "device_appleTV/device_appleTV.h"
#include "device_keyboard_mqtt/device_keyboard_mqtt.h"
#include "device_keyboard_ble/device_keyboard_ble.h"
#include "device_denonAvr/device_denonAvr.h"
// gui and keys
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/gui_irReceiver.h"
#include "gui_general_and_keys/gui_settings.h"
#include "gui_general_and_keys/gui_numpad.h"
#include "device_appleTV/gui_appleTV.h"
#include "device_smarthome/gui_smarthome.h"
#include "gui_general_and_keys/keys.h"
// scenes
#include "scenes/scene_allOff.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"
#include "scenes/scene_chromecast.h"
#include "scenes/sceneHandler.h"
// misc
#include "preferencesStorage.h"
#include "commandHandler.h"

void setup() {  
  setCpuFrequencyMhz(240); // Make sure ESP32 is running at full speed

  // --- Startup ---
  Serial.begin(115200);

  // Restore settings from internal flash memory
  init_preferences();
  // Button Pin definition
  init_keys();
  // Power Pin definition
  init_battery();
  // get wakeup reason
  init_sleep();
  // Pin definition
  init_userled();
  // init TFT
  init_tft();
  // setup the Inertial Measurement Unit (IMU) for motion detection
  // needs to be after init_tft()) because of I2C
  setup_IMU();
  // setup IR sender
  init_infraredSender();

  // register commands for the devices
  register_device_samsung();
  register_device_yamaha();
  register_device_smarthome();
  register_device_appleTV();
  #ifdef ENABLE_KEYBOARD_MQTT
  register_device_keyboard_mqtt();
  #endif
  #ifdef ENABLE_KEYBOARD_BLE
  register_device_keyboard_ble();
  #endif
  register_device_denon();
  register_specialCommands();

  // register the GUIs. They will be displayed in the order they are registered.
  register_gui_irReceiver();
  register_gui_settings();
  register_gui_numpad();
  register_gui_appleTV();
  register_gui_smarthome();
  // init GUI
  init_gui();
  gui_loop(); // Run the LVGL UI once before the loop takes over

  // register the scenes
  register_scene_allOff();
  register_scene_TV();
  register_scene_fireTV();
  register_scene_chromecast();
  setLabelCurrentScene();

  // init WiFi - needs to be after gui because WifiLabel must be available
  #ifdef ENABLE_WIFI_AND_MQTT
  init_mqtt();
  #endif

  Serial.print("Setup finished in ");
  Serial.print(millis());
  Serial.println("ms.");
}

// Loop ------------------------------------------------------------------------------------------------------------------------------------

void loop() { 

  // Update Backlight brightness
  update_backligthBrighness();

  // Update LVGL UI
  gui_loop();

  // Blink debug LED at 1 Hz
  update_userled();

  // Refresh IMU data at 10Hz
  static unsigned long IMUTaskTimer = millis();
  if(millis() - IMUTaskTimer >= 100){
    check_activity();
    IMUTaskTimer = millis();
  }

  // Update battery and BLE stats at 1Hz
  static unsigned long updateStatusTimer = millis();
  if(millis() - updateStatusTimer >= 1000){
    update_battery_stats();
    updateStatusTimer = millis();

    #ifdef ENABLE_BLUETOOTH
    // adjust this if you implement other bluetooth devices than the BLE keyboard
    #ifdef ENABLE_KEYBOARD_BLE
    update_keyboard_ble_status();
    #endif
    #endif

    // Serial.printf("heapSize: %lu, heapFree: %lu, heapMin: %lu, heapMax: %lu\r\n", ESP.getHeapSize(), ESP.getFreeHeap(), ESP.getMinFreeHeap(), ESP.getMaxAllocHeap());

  }

  // Keypad Handling
  keypad_loop();

  if (irReceiverEnabled) {
    infraredReceiver_loop();
  }

}
