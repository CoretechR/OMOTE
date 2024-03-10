#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__

#include <Arduino.h>
#include <string>
#include <list>
#include <map>

#include "device_keyboard_mqtt/device_keyboard_mqtt.h"
#include "device_keyboard_ble/device_keyboard_ble.h"

/*
  Depending on which keyboard is enabled (BLE or MQTT), we define KEYBOARD_UP, KEYBOARD_DOWN and so on.
  These defines are used in keys.cpp, gui*.cpp and commandHandler.cpp
  Example:
  If BLE  is enabled, then KEYBOARD_UP will be the same as KEYBOARD_BLE_UP
  If MQTT is enabled, then KEYBOARD_UP will be the same as KEYBOARD_MQTT_UP
  If none of them is enabled, then KEYBOARD_UP will be the same as KEYBOARD_UP
  Doing so you can switch between the keyboards without changing the UI code (keys.cpp, gui*.cpp and commandHandler.cpp)
  If you need something different than this behaviour, then you can change the defines below for KEYBOARD_UP, KEYBOARD_DOWN and so on,
  or you can of course change keys.cpp, gui*.cpp and commandHandler.cpp so that they directly use KEYBOARD_BLE_UP or KEYBOARD_MQTT_UP etc.
*/

#define KEYBOARD_DUMMY_UP                  "Keyboard_dummy_up"
#define KEYBOARD_DUMMY_DOWN                "Keyboard_dummy_down"
#define KEYBOARD_DUMMY_RIGHT               "Keyboard_dummy_right"
#define KEYBOARD_DUMMY_LEFT                "Keyboard_dummy_left"
#define KEYBOARD_DUMMY_SELECT              "Keyboard_dummy_select"
#define KEYBOARD_DUMMY_SENDSTRING          "Keyboard_dummy_sendstring"
#define KEYBOARD_DUMMY_BACK                "Keyboard_dummy_back"
#define KEYBOARD_DUMMY_HOME                "Keyboard_dummy_home"
#define KEYBOARD_DUMMY_MENU                "Keyboard_dummy_menu"
#define KEYBOARD_DUMMY_SCAN_PREVIOUS_TRACK "Keyboard_dummy_scan_previous_track"
#define KEYBOARD_DUMMY_REWIND_LONG         "Keyboard_dummy_rewind_long"
#define KEYBOARD_DUMMY_REWIND              "Keyboard_dummy_rewind"
#define KEYBOARD_DUMMY_PLAYPAUSE           "Keyboard_dummy_playpause"
#define KEYBOARD_DUMMY_FASTFORWARD         "Keyboard_dummy_fastforward"
#define KEYBOARD_DUMMY_FASTFORWARD_LONG    "Keyboard_dummy_fastforward_long"
#define KEYBOARD_DUMMY_SCAN_NEXT_TRACK     "Keyboard_dummy_scan_next_track"
#define KEYBOARD_DUMMY_MUTE                "Keyboard_dummy_mute"
#define KEYBOARD_DUMMY_VOLUME_INCREMENT    "Keyboard_dummy_volume_increment"
#define KEYBOARD_DUMMY_VOLUME_DECREMENT    "Keyboard_dummy_volume_decrement"

#if defined(ENABLE_KEYBOARD_BLE)
  #define KEYBOARD_PREFIX KEYBOARD_BLE_
#elif defined(ENABLE_KEYBOARD_MQTT)
  #define KEYBOARD_PREFIX KEYBOARD_MQTT_
#else
  // Of course keyboard commands will not work if neither BLE nor MQTT keyboard is enabled, but at least code will compile.
  // But you have to change keys.cpp, gui_numpad.cpp and commandHandler.cpp where keyboard commands are used so that a command can be executed successfully.
  // Search for "executeCommand(Key" to find them.
  #define KEYBOARD_PREFIX KEYBOARD_DUMMY_
#endif

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ## B
/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

#define KEYBOARD_UP                        PPCAT(KEYBOARD_PREFIX, UP)
#define KEYBOARD_DOWN                      PPCAT(KEYBOARD_PREFIX, DOWN)
#define KEYBOARD_RIGHT                     PPCAT(KEYBOARD_PREFIX, RIGHT)
#define KEYBOARD_LEFT                      PPCAT(KEYBOARD_PREFIX, LEFT)
#define KEYBOARD_SELECT                    PPCAT(KEYBOARD_PREFIX, SELECT)
#define KEYBOARD_SENDSTRING                PPCAT(KEYBOARD_PREFIX, SENDSTRING)
#define KEYBOARD_BACK                      PPCAT(KEYBOARD_PREFIX, BACK)
#define KEYBOARD_HOME                      PPCAT(KEYBOARD_PREFIX, HOME)
#define KEYBOARD_MENU                      PPCAT(KEYBOARD_PREFIX, MENU)
#define KEYBOARD_SCAN_PREVIOUS_TRACK       PPCAT(KEYBOARD_PREFIX, SCAN_PREVIOUS_TRACK)
#define KEYBOARD_REWIND_LONG               PPCAT(KEYBOARD_PREFIX, REWIND_LONG)
#define KEYBOARD_REWIND                    PPCAT(KEYBOARD_PREFIX, REWIND)
#define KEYBOARD_PLAYPAUSE                 PPCAT(KEYBOARD_PREFIX, PLAYPAUSE)
#define KEYBOARD_FASTFORWARD               PPCAT(KEYBOARD_PREFIX, FASTFORWARD)
#define KEYBOARD_FASTFORWARD_LONG          PPCAT(KEYBOARD_PREFIX, FASTFORWARD_LONG)
#define KEYBOARD_SCAN_NEXT_TRACK           PPCAT(KEYBOARD_PREFIX, SCAN_NEXT_TRACK)
#define KEYBOARD_MUTE                      PPCAT(KEYBOARD_PREFIX, MUTE)
#define KEYBOARD_VOLUME_INCREMENT          PPCAT(KEYBOARD_PREFIX, VOLUME_INCREMENT)
#define KEYBOARD_VOLUME_DECREMENT          PPCAT(KEYBOARD_PREFIX, VOLUME_DECREMENT)

// Test
// https://stackoverflow.com/questions/5256313/c-c-macro-string-concatenation
// #define STR(x) #x
// #define XSTR(x) STR(x)
// #pragma message "1 The value is: " XSTR(KEYBOARD_BLE_UP)
// #pragma message "2 The value is: " XSTR(KEYBOARD_MQTT_UP)
// #pragma message "3 The value is: " XSTR(KEYBOARD_UP)

#define MY_SPECIAL_COMMAND           "My_special_command"

enum commandHandlers {
  SPECIAL,
  SCENE,
  IR_GC,
  IR_NEC,
  IR_SAMSUNG,
  IR_SONY,
  IR_RC5,
  IR_DENON,
  #if ENABLE_WIFI_AND_MQTT == 1
  MQTT,
  #endif
  #ifdef ENABLE_KEYBOARD_BLE
  BLE_KEYBOARD,
  #endif
};

struct commandData {
  commandHandlers commandHandler;
  std::list<std::string> commandPayloads;
};

commandData makeCommandData(commandHandlers a, std::list<std::string> b);

extern std::map<std::string, commandData> commands;

void register_specialCommands();
void executeCommand(std::string command, std::string additionalPayload = "");

#endif /*__COMMANDHANDLER_H__*/
