#if (ENABLE_KEYBOARD_BLE == 1)

#include <string>
#include "keyboard_ble_hal_windows_linux.h"

void init_keyboardBLE_HAL() {};
bool keyboardBLE_isConnected_HAL() {return false;};
void keyboardBLE_end_HAL() {};
void keyboardBLE_write_HAL(uint8_t c) {};
void keyboardBLE_longpress_HAL(uint8_t c) {};
void keyboardBLE_home_HAL() {};
void keyboardBLE_sendString_HAL(const std::string &s) {};
void consumerControlBLE_write_HAL(const MediaKeyReport value) {};
void consumerControlBLE_longpress_HAL(const MediaKeyReport value) {};

#endif