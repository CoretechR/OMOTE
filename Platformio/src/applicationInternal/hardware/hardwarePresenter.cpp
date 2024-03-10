#include <string>
#include <list>
#include "applicationInternal/hardware/hardwarePresenter.h"
// for registering the callback to show received IR messages
#include "guis/gui_irReceiver.h"
// for registering the callback to show WiFi status
#include "applicationInternal/gui/guiBase.h"

// This include of "hardwareLayer.h" is the one and only link to folder "hardware". The file "hardwareLayer.h" does the differentiation between ESP32 and Windows/Linux.
// "hardwareLayer.h" includes all the other hardware header files as well. So everything from all hardware header files is available here - and only here.
// This include has to be here in "hardwarePresenter.cpp" and not in "hardwarePresenter.h". Otherwise the whole rest of the code would have access to the hardware too.
// The rest of the code is only allowed to use "hardwarePresenter.h".
#include "hardwareLayer.h"

// --- hardware general -------------------------------------------------------
void init_hardware_general(void) {
  init_hardware_general_HAL();
}

// --- preferences ------------------------------------------------------------
void init_preferences(void) {
  init_preferences_HAL();
};
void save_preferences(void) {
  save_preferences_HAL();
};
std::string get_currentScene() {
  return get_currentScene_HAL();
}
void set_currentScene(std::string aCurrentScene) {
  set_currentScene_HAL(aCurrentScene);
}
std::string get_currentGUIname() {
  return get_currentGUIname_HAL();
}
void set_currentGUIname(std::string aCurrentGUIname) {
  set_currentGUIname_HAL(aCurrentGUIname);
}

// --- user led ---------------------------------------------------------------
void init_userled(void) {
  init_userled_HAL();
}
void update_userled() {
  update_userled_HAL();
}

// --- battery ----------------------------------------------------------------
void init_battery(void) {
  init_battery_HAL();
}
void get_battery_status(int *battery_voltage, int *battery_percentage) {
  get_battery_status_HAL(battery_voltage, battery_percentage);
}

// --- sleep / IMU ------------------------------------------------------------
void init_sleep() {
  init_sleep_HAL();
};
void init_IMU() {
  init_IMU_HAL();
};
void check_activity() {
  check_activity_HAL();
};
void setLastActivityTimestamp() {
  setLastActivityTimestamp_HAL();
};
uint32_t get_sleepTimeout() {
  return get_sleepTimeout_HAL();
}
void set_sleepTimeout(uint32_t aSleepTimeout) {
  set_sleepTimeout_HAL(aSleepTimeout);
}
bool get_wakeupByIMUEnabled() {
  return get_wakeupByIMUEnabled_HAL();
}
void set_wakeupByIMUEnabled(bool aWakeupByIMUEnabled) {
  set_wakeupByIMUEnabled_HAL(aWakeupByIMUEnabled);
}

// --- keypad -----------------------------------------------------------------
void init_keys(void) {
  init_keys_HAL();  
}
keypad_key keypad_keys[keypad_maxkeys];
void getKeys(keypad_key *keys) {
  keys_getKeys_HAL(keys);
}

// --- IR sender --------------------------------------------------------------
void init_infraredSender(void) {
  init_infraredSender_HAL();  
}
void sendIRcode(IRprotocols protocol, std::list<std::string> commandPayloads, std::string additionalPayload) {
  sendIRcode_HAL(protocol, commandPayloads, additionalPayload);
}

// --- IR receiver ------------------------------------------------------------
void start_infraredReceiver(void) {
  start_infraredReceiver_HAL();
};
void shutdown_infraredReceiver(void) {
  shutdown_infraredReceiver_HAL();
};
void infraredReceiver_loop(void) {
  infraredReceiver_loop_HAL();
};
bool get_irReceiverEnabled() {
  return get_irReceiverEnabled_HAL();
}
void set_irReceiverEnabled(bool aIrReceiverEnabled) {
  if (aIrReceiverEnabled) {
    set_showNewIRmessage_cb_HAL(&showNewIRmessage_cb);
  } else {
    set_showNewIRmessage_cb_HAL(NULL);
  }
  set_irReceiverEnabled_HAL(aIrReceiverEnabled);
}

// --- BLE keyboard -----------------------------------------------------------
#if (ENABLE_KEYBOARD_BLE == 1)
void init_keyboardBLE() {
  init_keyboardBLE_HAL();
}
// used by "device_keyboard_ble.cpp", "sleep.cpp"
bool keyboardBLE_isConnected() {
  return keyboardBLE_isConnected_HAL();
}
void keyboardBLE_end() {
  keyboardBLE_end_HAL();
}
void keyboardBLE_write(uint8_t c) {
  keyboardBLE_write_HAL(c);
}
void keyboardBLE_longpress(uint8_t c) {
  keyboardBLE_longpress_HAL(c);
}
void keyboardBLE_home() {
  keyboardBLE_home_HAL();
}
void keyboardBLE_sendString(const std::string &s) {
  keyboardBLE_sendString_HAL(s);
}
void consumerControlBLE_write(const MediaKeyReport value) {
  consumerControlBLE_write_HAL(value);
}
void consumerControlBLE_longpress(const MediaKeyReport value) {
  consumerControlBLE_longpress_HAL(value);
}
#endif

// --- tft --------------------------------------------------------------------
void update_backligthBrighness(void) {
  update_backligthBrighness_HAL();
};
uint8_t get_backlightBrightness() {
  return get_backlightBrightness_HAL();
}
void set_backlightBrightness(uint8_t aBacklightBrightness){
  set_backlightBrightness_HAL(aBacklightBrightness);
}

// --- lvgl -------------------------------------------------------------------
void init_lvgl_hardware() {
  init_lvgl_HAL();
};

// --- WiFi / MQTT ------------------------------------------------------------
#if (ENABLE_WIFI_AND_MQTT == 1)
void init_mqtt(void) {
  set_showWiFiconnected_cb_HAL(&showWiFiConnected_cb);
  init_mqtt_HAL();
}
// used by "commandHandler.cpp", "sleep.cpp"
bool getIsWifiConnected() {
  return getIsWifiConnected_HAL();
}
bool publishMQTTMessage(const char *topic, const char *payload) {
  return publishMQTTMessage_HAL(topic, payload);
}
void wifiStop() {
  wifiStop_HAL();
}
#endif

// --- memory usage -----------------------------------------------------------
void get_heapUsage(unsigned long *heapSize, unsigned long *freeHeap, unsigned long *maxAllocHeap, unsigned long *minFreeHeap) {
  get_heapUsage_HAL(heapSize, freeHeap, maxAllocHeap, minFreeHeap);
}
