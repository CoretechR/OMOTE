#ifndef __DEVICE_KEYBOARD_BLE_H__
#define __DEVICE_KEYBOARD_BLE_H__

// Advertising is started automatically.
// As soon as a device is connected, a small indicator in the top left corner of the screen will appear

#define ENABLE_KEYBOARD_BLE // Comment out to disable BLE

#ifdef ENABLE_KEYBOARD_BLE

#if defined(ENABLE_KEYBOARD_BLE) && !(ENABLE_BLUETOOTH == 1)
static_assert(false, "You have to use \"-D ENABLE_BLUETOOTH=1\" in \"platformio.ini\" when having \"#define ENABLE_KEYBOARD_BLE\"");
#endif

#include <BleKeyboard.h>

#define KEYBOARD_BLE_UP                  "Keyboard_ble_up"
#define KEYBOARD_BLE_DOWN                "Keyboard_ble_down"
#define KEYBOARD_BLE_RIGHT               "Keyboard_ble_right"
#define KEYBOARD_BLE_LEFT                "Keyboard_ble_left"
#define KEYBOARD_BLE_SELECT              "Keyboard_ble_select"
#define KEYBOARD_BLE_SENDSTRING          "Keyboard_ble_sendstring"
#define KEYBOARD_BLE_BACK                "Keyboard_ble_back"
#define KEYBOARD_BLE_HOME                "Keyboard_ble_home"
#define KEYBOARD_BLE_MENU                "Keyboard_ble_menu"
#define KEYBOARD_BLE_SCAN_PREVIOUS_TRACK "Keyboard_ble_scan_previous_track"
#define KEYBOARD_BLE_REWIND_LONG         "Keyboard_ble_rewind_long"
#define KEYBOARD_BLE_REWIND              "Keyboard_ble_rewind"
#define KEYBOARD_BLE_PLAYPAUSE           "Keyboard_ble_playpause"
#define KEYBOARD_BLE_FASTFORWARD         "Keyboard_ble_fastforward"
#define KEYBOARD_BLE_FASTFORWARD_LONG    "Keyboard_ble_fastforward_long"
#define KEYBOARD_BLE_SCAN_NEXT_TRACK     "Keyboard_ble_scan_next_track"
#define KEYBOARD_BLE_MUTE                "Keyboard_ble_mute"
#define KEYBOARD_BLE_VOLUME_INCREMENT    "Keyboard_ble_volume_increment"
#define KEYBOARD_BLE_VOLUME_DECREMENT    "Keyboard_ble_volume_decrement"

extern BleKeyboard bleKeyboard;

void register_device_keyboard_ble();
void keyboard_ble_executeCommand(std::string command, std::string additionalPayload = "");
void update_keyboard_ble_status();

#endif

#endif /*__DEVICE_KEYBOARD_BLE_H__*/
