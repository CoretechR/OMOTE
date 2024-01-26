#include <Arduino.h>

// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include "hardware/tft.h"
#include "hardware/infrared_sender.h"
#include "hardware/infrared_receiver.h"
#include "hardware/mqtt.h"
#include "hardware/keys.h"
#include "hardware/sleep.h"
#include "hardware/battery.h"
#include "hardware/user_led.h"
#include "device_samsungTV/device_samsungTV.h"
#include "device_yamahaAmp/device_yamahaAmp.h"
#include "device_appleTV/device_appleTV.h"
#include "device_smarthome/device_smarthome.h"
#include "device_keyboard_mqtt/device_keyboard_mqtt.h"
#include "device_keyboard_ble/device_keyboard_ble.h"
#include "gui_general/gui.h"
#include "preferences_storage.h"
#include "commandHandler.h"

void setup() {  
  setCpuFrequencyMhz(240); // Make sure ESP32 is running at full speed

  // --- Startup ---
  Serial.begin(115200);

  // Restore settings from internal flash memory
  init_preferences();

  // Button Pin Definition
  init_keys();

  // Power Pin Definition
  init_battery();

  init_sleep();

  init_userled();

  // init TFT
  init_tft();

  // init GUI
  init_gui();

  // init WiFi
  #ifdef ENABLE_WIFI_AND_MQTT
  init_mqtt();
  #endif

  // needs to be here (after init_tft) because of I2C
  setup_IMU();

  // Setup IR
  init_infrared();

  // setup command list
  init_samsung_commands();
  init_yamaha_commands();
  init_smarthome_mqtt_commands();
  init_appleTV_commands();
  #ifdef ENABLE_KEYBOARD_MQTT
  init_keyboard_mqtt_commands();
  #endif
  #ifdef ENABLE_KEYBOARD_BLE
  init_keyboard_ble_commands();
  #endif
  init_deviceIndependantCommands();

  gui_loop(); // Run the LVGL UI once before the loop takes over

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

  // IR Test
  //tft.drawString("IR Command: ", 10, 90, 1);
  //decode_results results;
  //if (IrReceiver.decode(&results)) {
  //  //tft.drawString(String(results.command) + "        ", 80, 90, 1);
  //  IrReceiver.resume(); // Enable receiving of the next value
  //}
}