#include <string>
#include <list>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "applicationInternal/commandHandler.h"
#include "applicationInternal/scenes/sceneHandler.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "devices/misc/device_specialCommands.h"

uint16_t KEYBOARD_DUMMY_UP                  ; //"Keyboard_dummy_up"
uint16_t KEYBOARD_DUMMY_DOWN                ; //"Keyboard_dummy_down"
uint16_t KEYBOARD_DUMMY_RIGHT               ; //"Keyboard_dummy_right"
uint16_t KEYBOARD_DUMMY_LEFT                ; //"Keyboard_dummy_left"
uint16_t KEYBOARD_DUMMY_SELECT              ; //"Keyboard_dummy_select"
uint16_t KEYBOARD_DUMMY_SENDSTRING          ; //"Keyboard_dummy_sendstring"
uint16_t KEYBOARD_DUMMY_BACK                ; //"Keyboard_dummy_back"
uint16_t KEYBOARD_DUMMY_HOME                ; //"Keyboard_dummy_home"
uint16_t KEYBOARD_DUMMY_MENU                ; //"Keyboard_dummy_menu"
uint16_t KEYBOARD_DUMMY_SCAN_PREVIOUS_TRACK ; //"Keyboard_dummy_scan_previous_track"
uint16_t KEYBOARD_DUMMY_REWIND_LONG         ; //"Keyboard_dummy_rewind_long"
uint16_t KEYBOARD_DUMMY_REWIND              ; //"Keyboard_dummy_rewind"
uint16_t KEYBOARD_DUMMY_PLAYPAUSE           ; //"Keyboard_dummy_playpause"
uint16_t KEYBOARD_DUMMY_FASTFORWARD         ; //"Keyboard_dummy_fastforward"
uint16_t KEYBOARD_DUMMY_FASTFORWARD_LONG    ; //"Keyboard_dummy_fastforward_long"
uint16_t KEYBOARD_DUMMY_SCAN_NEXT_TRACK     ; //"Keyboard_dummy_scan_next_track"
uint16_t KEYBOARD_DUMMY_MUTE                ; //"Keyboard_dummy_mute"
uint16_t KEYBOARD_DUMMY_VOLUME_INCREMENT    ; //"Keyboard_dummy_volume_increment"
uint16_t KEYBOARD_DUMMY_VOLUME_DECREMENT    ; //"Keyboard_dummy_volume_decrement"

uint16_t KEYBOARD_UP                        ; //PPCAT(KEYBOARD_PREFIX, UP)
uint16_t KEYBOARD_DOWN                      ; //PPCAT(KEYBOARD_PREFIX, DOWN)
uint16_t KEYBOARD_RIGHT                     ; //PPCAT(KEYBOARD_PREFIX, RIGHT)
uint16_t KEYBOARD_LEFT                      ; //PPCAT(KEYBOARD_PREFIX, LEFT)
uint16_t KEYBOARD_SELECT                    ; //PPCAT(KEYBOARD_PREFIX, SELECT)
uint16_t KEYBOARD_SENDSTRING                ; //PPCAT(KEYBOARD_PREFIX, SENDSTRING)
uint16_t KEYBOARD_BACK                      ; //PPCAT(KEYBOARD_PREFIX, BACK)
uint16_t KEYBOARD_HOME                      ; //PPCAT(KEYBOARD_PREFIX, HOME)
uint16_t KEYBOARD_MENU                      ; //PPCAT(KEYBOARD_PREFIX, MENU)
uint16_t KEYBOARD_SCAN_PREVIOUS_TRACK       ; //PPCAT(KEYBOARD_PREFIX, SCAN_PREVIOUS_TRACK)
uint16_t KEYBOARD_REWIND_LONG               ; //PPCAT(KEYBOARD_PREFIX, REWIND_LONG)
uint16_t KEYBOARD_REWIND                    ; //PPCAT(KEYBOARD_PREFIX, REWIND)
uint16_t KEYBOARD_PLAYPAUSE                 ; //PPCAT(KEYBOARD_PREFIX, PLAYPAUSE)
uint16_t KEYBOARD_FASTFORWARD               ; //PPCAT(KEYBOARD_PREFIX, FASTFORWARD)
uint16_t KEYBOARD_FASTFORWARD_LONG          ; //PPCAT(KEYBOARD_PREFIX, FASTFORWARD_LONG)
uint16_t KEYBOARD_SCAN_NEXT_TRACK           ; //PPCAT(KEYBOARD_PREFIX, SCAN_NEXT_TRACK)
uint16_t KEYBOARD_MUTE                      ; //PPCAT(KEYBOARD_PREFIX, MUTE)
uint16_t KEYBOARD_VOLUME_INCREMENT          ; //PPCAT(KEYBOARD_PREFIX, VOLUME_INCREMENT)
uint16_t KEYBOARD_VOLUME_DECREMENT          ; //PPCAT(KEYBOARD_PREFIX, VOLUME_DECREMENT)

std::map<uint16_t, commandData> commands;

uint16_t uniqueCommandID = 0;

// we don't yet have a command id
void register_command(uint16_t *command, commandData aCommandData) {
  *command = uniqueCommandID;
  uniqueCommandID++;

  commands[*command] = aCommandData;
}
// we already have a command id. Only used by BLE keyboard
void register_command_withID(uint16_t command, commandData aCommandData) {
  commands[command] = aCommandData;
}
// only get a unique ID. used by KEYBOARD_DUMMY, COMMAND_UNKNOWN and BLE keyboard
void get_uniqueCommandID(uint16_t *command) {
  *command = uniqueCommandID;
  uniqueCommandID++;
}

void register_keyboardCommands() {
  get_uniqueCommandID(&KEYBOARD_DUMMY_UP                  );
  get_uniqueCommandID(&KEYBOARD_DUMMY_DOWN                );
  get_uniqueCommandID(&KEYBOARD_DUMMY_RIGHT               );
  get_uniqueCommandID(&KEYBOARD_DUMMY_LEFT                );
  get_uniqueCommandID(&KEYBOARD_DUMMY_SELECT              );
  get_uniqueCommandID(&KEYBOARD_DUMMY_SENDSTRING          );
  get_uniqueCommandID(&KEYBOARD_DUMMY_BACK                );
  get_uniqueCommandID(&KEYBOARD_DUMMY_HOME                );
  get_uniqueCommandID(&KEYBOARD_DUMMY_MENU                );
  get_uniqueCommandID(&KEYBOARD_DUMMY_SCAN_PREVIOUS_TRACK );
  get_uniqueCommandID(&KEYBOARD_DUMMY_REWIND_LONG         );
  get_uniqueCommandID(&KEYBOARD_DUMMY_REWIND              );
  get_uniqueCommandID(&KEYBOARD_DUMMY_PLAYPAUSE           );
  get_uniqueCommandID(&KEYBOARD_DUMMY_FASTFORWARD         );
  get_uniqueCommandID(&KEYBOARD_DUMMY_FASTFORWARD_LONG    );
  get_uniqueCommandID(&KEYBOARD_DUMMY_SCAN_NEXT_TRACK     );
  get_uniqueCommandID(&KEYBOARD_DUMMY_MUTE                );
  get_uniqueCommandID(&KEYBOARD_DUMMY_VOLUME_INCREMENT    );
  get_uniqueCommandID(&KEYBOARD_DUMMY_VOLUME_DECREMENT    );

#if (ENABLE_KEYBOARD_BLE == 1)
  KEYBOARD_UP                  = KEYBOARD_BLE_UP;
  KEYBOARD_DOWN                = KEYBOARD_BLE_DOWN;
  KEYBOARD_RIGHT               = KEYBOARD_BLE_RIGHT;
  KEYBOARD_LEFT                = KEYBOARD_BLE_LEFT;
  KEYBOARD_SELECT              = KEYBOARD_BLE_SELECT;
  KEYBOARD_SENDSTRING          = KEYBOARD_BLE_SENDSTRING;
  KEYBOARD_BACK                = KEYBOARD_BLE_BACK;
  KEYBOARD_HOME                = KEYBOARD_BLE_HOME;
  KEYBOARD_MENU                = KEYBOARD_BLE_MENU;
  KEYBOARD_SCAN_PREVIOUS_TRACK = KEYBOARD_BLE_SCAN_PREVIOUS_TRACK;
  KEYBOARD_REWIND_LONG         = KEYBOARD_BLE_REWIND_LONG;
  KEYBOARD_REWIND              = KEYBOARD_BLE_REWIND;
  KEYBOARD_PLAYPAUSE           = KEYBOARD_BLE_PLAYPAUSE;
  KEYBOARD_FASTFORWARD         = KEYBOARD_BLE_FASTFORWARD;
  KEYBOARD_FASTFORWARD_LONG    = KEYBOARD_BLE_FASTFORWARD_LONG;
  KEYBOARD_SCAN_NEXT_TRACK     = KEYBOARD_BLE_SCAN_NEXT_TRACK;
  KEYBOARD_MUTE                = KEYBOARD_BLE_MUTE;
  KEYBOARD_VOLUME_INCREMENT    = KEYBOARD_BLE_VOLUME_INCREMENT;
  KEYBOARD_VOLUME_DECREMENT    = KEYBOARD_BLE_VOLUME_DECREMENT;
#elif (ENABLE_KEYBOARD_MQTT == 1)
  KEYBOARD_UP                  = KEYBOARD_MQTT_UP;
  KEYBOARD_DOWN                = KEYBOARD_MQTT_DOWN;
  KEYBOARD_RIGHT               = KEYBOARD_MQTT_RIGHT;
  KEYBOARD_LEFT                = KEYBOARD_MQTT_LEFT;
  KEYBOARD_SELECT              = KEYBOARD_MQTT_SELECT;
  KEYBOARD_SENDSTRING          = KEYBOARD_MQTT_SENDSTRING;
  KEYBOARD_BACK                = KEYBOARD_MQTT_BACK;
  KEYBOARD_HOME                = KEYBOARD_MQTT_HOME;
  KEYBOARD_MENU                = KEYBOARD_MQTT_MENU;
  KEYBOARD_SCAN_PREVIOUS_TRACK = KEYBOARD_MQTT_SCAN_PREVIOUS_TRACK;
  KEYBOARD_REWIND_LONG         = KEYBOARD_MQTT_REWIND_LONG;
  KEYBOARD_REWIND              = KEYBOARD_MQTT_REWIND;
  KEYBOARD_PLAYPAUSE           = KEYBOARD_MQTT_PLAYPAUSE;
  KEYBOARD_FASTFORWARD         = KEYBOARD_MQTT_FASTFORWARD;
  KEYBOARD_FASTFORWARD_LONG    = KEYBOARD_MQTT_FASTFORWARD_LONG;
  KEYBOARD_SCAN_NEXT_TRACK     = KEYBOARD_MQTT_SCAN_NEXT_TRACK;
  KEYBOARD_MUTE                = KEYBOARD_MQTT_MUTE;
  KEYBOARD_VOLUME_INCREMENT    = KEYBOARD_MQTT_VOLUME_INCREMENT;
  KEYBOARD_VOLUME_DECREMENT    = KEYBOARD_MQTT_VOLUME_DECREMENT;
#else
  // Of course keyboard commands will not work if neither BLE nor MQTT keyboard is enabled, but at least code will compile.
  // But you have to change keys.cpp, gui_numpad.cpp and commandHandler.cpp where keyboard commands are used so that a command can be executed successfully.
  // Search for "executeCommand(Key" to find them.
  KEYBOARD_UP                  = KEYBOARD_DUMMY_UP;
  KEYBOARD_DOWN                = KEYBOARD_DUMMY_DOWN;
  KEYBOARD_RIGHT               = KEYBOARD_DUMMY_RIGHT;
  KEYBOARD_LEFT                = KEYBOARD_DUMMY_LEFT;
  KEYBOARD_SELECT              = KEYBOARD_DUMMY_SELECT;
  KEYBOARD_SENDSTRING          = KEYBOARD_DUMMY_SENDSTRING;
  KEYBOARD_BACK                = KEYBOARD_DUMMY_BACK;
  KEYBOARD_HOME                = KEYBOARD_DUMMY_HOME;
  KEYBOARD_MENU                = KEYBOARD_DUMMY_MENU;
  KEYBOARD_SCAN_PREVIOUS_TRACK = KEYBOARD_DUMMY_SCAN_PREVIOUS_TRACK;
  KEYBOARD_REWIND_LONG         = KEYBOARD_DUMMY_REWIND_LONG;
  KEYBOARD_REWIND              = KEYBOARD_DUMMY_REWIND;
  KEYBOARD_PLAYPAUSE           = KEYBOARD_DUMMY_PLAYPAUSE;
  KEYBOARD_FASTFORWARD         = KEYBOARD_DUMMY_FASTFORWARD;
  KEYBOARD_FASTFORWARD_LONG    = KEYBOARD_DUMMY_FASTFORWARD_LONG;
  KEYBOARD_SCAN_NEXT_TRACK     = KEYBOARD_DUMMY_SCAN_NEXT_TRACK;
  KEYBOARD_MUTE                = KEYBOARD_DUMMY_MUTE;
  KEYBOARD_VOLUME_INCREMENT    = KEYBOARD_DUMMY_VOLUME_INCREMENT;
  KEYBOARD_VOLUME_DECREMENT    = KEYBOARD_DUMMY_VOLUME_DECREMENT;
#endif

}

commandData makeCommandData(commandHandlers a, std::list<std::string> b) {
  commandData c = {a, b};
  return c;
}

std::string convertStringListToString(std::list<std::string> listOfStrings) {
  std::string result; 
  for(const auto &word : listOfStrings) { 
	  result += word + ","; 
  } 
  return result;
}

void executeCommandWithData(uint16_t command, commandData commandData, std::string additionalPayload = "") {
  switch (commandData.commandHandler) {
    case IR: {
      // Serial.printf("  generic IR, payloads %s\r\n", convertStringListToString(commandData.commandPayloads).c_str());

      // we received a comma separated list of strings
      // the first string is the IR protocol, the second is the payload to be sent
      std::list<std::string>::iterator it = commandData.commandPayloads.begin();
      // get protocol and erase first element in list
      std::string protocol = *it;
      it = commandData.commandPayloads.erase(it);
      // Serial.printf("  protocol %s, payload %s\r\n", protocol.c_str(), convertStringListToString(commandData.commandPayloads).c_str());
      
      sendIRcode((IRprotocols)std::stoi(protocol), commandData.commandPayloads, additionalPayload);
      break;
    }

    #if (ENABLE_WIFI_AND_MQTT == 1)
    case MQTT: {
      auto current = commandData.commandPayloads.begin();
      std::string topic = *current;
      std::string payload;
      if (additionalPayload == "") {
        current = std::next(current, 1);
        payload = *current;
      } else {
        payload = additionalPayload;
      }
      Serial.printf("execute: will send MQTT, topic '%s', payload '%s'\r\n", topic.c_str(), payload.c_str());
      publishMQTTMessage(topic.c_str(), payload.c_str());
      break;
    }
    #endif

    #if (ENABLE_KEYBOARD_BLE == 1)
    case BLE_KEYBOARD: {
      // the real command for the BLE keyboard is the first element in payload
      auto current = commandData.commandPayloads.begin();
      uint16_t command = std::stoi(*current);
      std::string payload = "";
      if (additionalPayload != "") {
        payload = additionalPayload;
      }
      Serial.printf("execute: will send BLE keyboard command, command '%u', payload '%s'\r\n", command, payload.c_str());
      keyboard_ble_executeCommand(command, payload);
      break;
    }
    #endif

    case SCENE: {
      // let the sceneHandler do the scene stuff
      Serial.printf("execute: will send scene command to the sceneHandler\r\n");
      handleScene(command, commandData, additionalPayload);
      break;
    }
    
    case SPECIAL: {
      if (command == MY_SPECIAL_COMMAND) {
        // do your special command here
        Serial.printf("execute: could execute a special command here, if you define one\r\n");

      }
      break;
    }
  }
}

void executeCommand(uint16_t command, std::string additionalPayload) {
  try {
    if (commands.count(command) > 0) {
      Serial.printf("command: will execute command '%u' with additionalPayload '%s'\r\n", command, additionalPayload.c_str());
      executeCommandWithData(command, commands.at(command), additionalPayload);
    } else {
      Serial.printf("command: command '%u' not found\r\n", command);
    }
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("executeCommand: internal error, command not registered\r\n");
  }
}
