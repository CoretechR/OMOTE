#if (ENABLE_KEYBOARD_BLE == 1)

#include "lib/ESP32-BLE-Keyboard/BleKeyboard.h"
#include "battery_hal_esp32.h"

BleKeyboard bleKeyboard("OMOTE Keyboard", "CoretechR");

void init_keyboardBLE_HAL() {
  int battery_voltage;
  int battery_percentage;
  boolean battery_ischarging;
  get_battery_status_HAL(&battery_voltage, &battery_percentage, &battery_ischarging);
  bleKeyboard.setBatteryLevel(battery_percentage);
  bleKeyboard.begin();
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
