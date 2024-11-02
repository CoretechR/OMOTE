#if (ENABLE_KEYBOARD_BLE == 1)

#include "lib/ESP32-BLE-Keyboard/BleKeyboard.h"
#include "battery_hal_esp32.h"
#include "keyboard_ble_hal_esp32.h"

BleKeyboard bleKeyboard("OMOTE Keyboard", "CoretechR");

void keyboardBLE_startAdvertisingForAll_HAL() {
  bleKeyboard.startAdvertisingForAll();
}

void keyboardBLE_startAdvertisingWithWhitelist_HAL(std::string peersAllowed) {
  bleKeyboard.startAdvertisingWithWhitelist(peersAllowed);
}

void keyboardBLE_startAdvertisingDirected_HAL(std::string peerAddress, bool isRandomAddress) {
  bleKeyboard.startAdvertisingDirected(peerAddress, isRandomAddress);
}

void keyboardBLE_stopAdvertising_HAL() {
  bleKeyboard.stopAdvertising();
}

void keyboardBLE_printConnectedClients_HAL() {
  bleKeyboard.printConnectedClients();
}

void keyboardBLE_disconnectAllClients_HAL() {
  bleKeyboard.disconnectAllClients();
}

void keyboardBLE_printBonds_HAL() {
  bleKeyboard.printBonds();
}

std::string keyboardBLE_getBonds_HAL() {
  return bleKeyboard.getBonds();
}

void keyboardBLE_deleteBonds_HAL() {
  bleKeyboard.deleteBonds();
}

bool keyboardBLE_forceConnectionToAddress_HAL(std::string peerAddress) {
  return bleKeyboard.forceConnectionToAddress(peerAddress);
}

tAnnounceBLEmessage_cb thisAnnounceBLEmessage_cb = NULL;
void set_announceBLEmessage_cb_HAL(tAnnounceBLEmessage_cb pAnnounceBLEmessage_cb) {
  // this is the callback in the commandHandler that we call from here
  thisAnnounceBLEmessage_cb = pAnnounceBLEmessage_cb;  
}

void keyboardBLE_BLEkeyboardMessage_cb(std::string message) {
  // this callback is called from BLEKeyboard.cpp
  if (thisAnnounceBLEmessage_cb != NULL) {
    thisAnnounceBLEmessage_cb(message);
  }
}

void init_keyboardBLE_HAL() {
  int battery_voltage;
  int battery_percentage;
  boolean battery_ischarging;
  get_battery_status_HAL(&battery_voltage, &battery_percentage, &battery_ischarging);

  bleKeyboard.set_BLEKeyboardMessage_cb(&keyboardBLE_BLEkeyboardMessage_cb);
  bleKeyboard.setBatteryLevel(battery_percentage);
  bleKeyboard.begin();
  // In case only one peer is bonded, startAdvertisingForAll() is called on initialisation
  bleKeyboard.startAdvertisingIfExactlyOneBondExists();
}

bool keyboardBLE_isAdvertising_HAL() {
  return bleKeyboard.isAdvertising();
}

bool keyboardBLE_isConnected_HAL() {
  return bleKeyboard.isConnected();
}

void keyboardBLE_end_HAL() {
  bleKeyboard.end();
}
    
void keyboardBLE_write_HAL(uint8_t c) {
  bleKeyboard.write(c);
}

void keyboardBLE_longpress_HAL(uint8_t c) {
  bleKeyboard.press(c);
  delay(1000);
  bleKeyboard.release(c);
}

void keyboardBLE_home_HAL() {
  bleKeyboard.press(KEY_LEFT_ALT);
  bleKeyboard.press(KEY_ESC);
  bleKeyboard.releaseAll();
}

void keyboardBLE_sendString_HAL(const std::string &s) {
  bleKeyboard.print(s.c_str());
}

void consumerControlBLE_write_HAL(const MediaKeyReport value) {
  bleKeyboard.write(value);
}

void consumerControlBLE_longpress_HAL(const MediaKeyReport value) {
  bleKeyboard.press(value);
  delay(1000);
  bleKeyboard.release(value);
}

#endif
