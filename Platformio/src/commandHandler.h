#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__

#include <Arduino.h>
#include <string>
#include <list>
#include <map>

#include "device_keyboard_mqtt.h"
#include "device_keyboard_ble.h"

/*
  Depending on which keyboard you want to use, use KEYBOARD_BLE_* or KEYBOARD_MQTT_* in the second column
  Doing so you can switch between the keyboards without changing the UI code (keys.cpp and gui*.cpp)
*/

#define KEYBOARD_UP                  KEYBOARD_MQTT_UP
#define KEYBOARD_DOWN                KEYBOARD_MQTT_DOWN
#define KEYBOARD_RIGHT               KEYBOARD_MQTT_RIGHT
#define KEYBOARD_LEFT                KEYBOARD_MQTT_LEFT
#define KEYBOARD_SELECT              KEYBOARD_MQTT_SELECT
#define KEYBOARD_SENDSTRING          KEYBOARD_MQTT_SENDSTRING
#define KEYBOARD_BACK                KEYBOARD_MQTT_BACK
#define KEYBOARD_HOME                KEYBOARD_MQTT_HOME
#define KEYBOARD_MENU                KEYBOARD_MQTT_MENU
#define KEYBOARD_SCAN_PREVIOUS_TRACK KEYBOARD_MQTT_SCAN_PREVIOUS_TRACK
#define KEYBOARD_REWIND_LONG         KEYBOARD_MQTT_REWIND_LONG
#define KEYBOARD_REWIND              KEYBOARD_MQTT_REWIND
#define KEYBOARD_PLAYPAUSE           KEYBOARD_MQTT_PLAYPAUSE
#define KEYBOARD_FASTFORWARD         KEYBOARD_MQTT_FASTFORWARD
#define KEYBOARD_FASTFORWARD_LONG    KEYBOARD_MQTT_FASTFORWARD_LONG
#define KEYBOARD_SCAN_NEXT_TRACK     KEYBOARD_MQTT_SCAN_NEXT_TRACK
#define KEYBOARD_MUTE                KEYBOARD_MQTT_MUTE
#define KEYBOARD_VOLUME_INCREMENT    KEYBOARD_MQTT_VOLUME_INCREMENT
#define KEYBOARD_VOLUME_DECREMENT    KEYBOARD_MQTT_VOLUME_DECREMENT

#define ALLDEVICES_POWER_TOGGLE      "AllDevices_power_toggle"
#define SCENE_TV                     "Scene_tv"
#define SCENE_FIRETV                 "Scene_firetv"
#define SCENE_CHROMECAST             "Scene_chromecast"

extern byte allDevsPowered;
extern String currentScene; // Current scene that is active

enum commandHandlers {
  SPECIAL,
  IR_GC,
  IR_NEC,
  IR_SAMSUNG,
  #ifdef ENABLE_KEYBOARD_MQTT
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

void init_deviceIndependantCommands();
void executeCommand(std::string command, std::string additionalPayload = "");

#endif /*__COMMANDHANDLER_H__*/
