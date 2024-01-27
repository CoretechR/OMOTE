#include <string>
#include <sstream>
#include "hardware/infrared_sender.h"
#include "hardware/mqtt.h"
#include "device_samsungTV/device_samsungTV.h"
#include "device_yamahaAmp/device_yamahaAmp.h"
#include "device_keyboard_mqtt/device_keyboard_mqtt.h"
#include "device_keyboard_ble/device_keyboard_ble.h"
#include "commandHandler.h"

std::map<std::string, commandData> commands;
byte allDevsPowered = 0;
String currentScene = "";

commandData makeCommandData(commandHandlers a, std::list<std::string> b) {
  commandData c = {a, b};
  return c;
}

void init_deviceIndependantCommands() {
  // put commands here if you want
  commands[ALLDEVICES_POWER_TOGGLE] = makeCommandData(SPECIAL, {""});
  commands[SCENE_TV]                = makeCommandData(SPECIAL, {""});
  commands[SCENE_FIRETV]            = makeCommandData(SPECIAL, {""});
  commands[SCENE_CHROMECAST]        = makeCommandData(SPECIAL, {""});
}

void executeCommandWithData(std::string command, commandData commandData, std::string additionalPayload = "") {
  switch (commandData.commandHandler) {
    case IR_GC: {
      auto current = commandData.commandPayloads.begin();
      std::string arrayStr = *current;
      // first create array of needed size
      std::string::difference_type size = std::count(arrayStr.begin(), arrayStr.end(), ',');
      size += 1;
      uint16_t *buf = new uint16_t[size];
      // now get comma separated values and fill array
      int pos = 0;
      std::stringstream ss(arrayStr);
      while(ss.good())  {
        std::string dataStr;
        std::getline(ss, dataStr, ',');
        // https://cplusplus.com/reference/string/stoull/
        std::string::size_type sz = 0;   // alias of size_t
        const uint64_t data = std::stoull(dataStr, &sz, 0);
        // Serial.printf("  next string value %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
        buf[pos] = data;
        pos += 1;
      }
      Serial.printf("execute: will send IR GC, array size %d\r\n", size);
      IrSender.sendGC(buf, size);
      delete [] buf;
      break;
    }

    case IR_NEC: {
      auto current = commandData.commandPayloads.begin();
      std::string dataStr = *current;
      // https://cplusplus.com/reference/string/stoull/
      std::string::size_type sz = 0;   // alias of size_t
      const uint64_t data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR NEC, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendNEC(data);
      break;
    }

    case IR_SAMSUNG: {
      auto current = commandData.commandPayloads.begin();
      std::string dataStr = *current;
      // https://cplusplus.com/reference/string/stoull/
      std::string::size_type sz = 0;   // alias of size_t
      const uint64_t data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR SAMSUNG, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendSAMSUNG(data);
      break;
    }

    case IR_SONY: {
      std::string::size_type sz = 0;   // alias of size_t
      uint64_t data;
      if (commandData.commandPayloads.empty() && (additionalPayload == "")) {
        Serial.printf("execute: cannot send IR SONY, because both data and payload are empty\r\n");
      } else {
        if (additionalPayload != "") {
          data = std::stoull(additionalPayload, &sz, 0);
        } else {
          auto current = commandData.commandPayloads.begin();
          data = std::stoull(*current, &sz, 0);
        }
        Serial.printf("execute: will send IR SONY, data (%" PRIu64 ")\r\n", data);
        IrSender.sendSony(data, 15);
      }
      break;
    }

    case IR_RC5: {
      std::string::size_type sz = 0;   // alias of size_t
      uint64_t data;
      if (commandData.commandPayloads.empty() && (additionalPayload == "")) {
        Serial.printf("execute: cannot send IR RC5, because both data and payload are empty\r\n");
      } else {
        if (additionalPayload != "") {
          data = std::stoull(additionalPayload, &sz, 0);
        } else {
          auto current = commandData.commandPayloads.begin();
          data = std::stoull(*current, &sz, 0);
        }
        Serial.printf("execute: will send IR RC5, data (%" PRIu64 ")\r\n", data);
        IrSender.sendRC5(IrSender.encodeRC5X(0x00, data));
      }
      break;
    }

    #ifdef ENABLE_WIFI_AND_MQTT
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

    #ifdef ENABLE_KEYBOARD_BLE
    case BLE_KEYBOARD: {
      auto current = commandData.commandPayloads.begin();
      std::string command = *current;
      std::string payload = "";
      if (additionalPayload != "") {
        payload = additionalPayload;
      }
      Serial.printf("execute: will send BLE keyboard command, command '%s', payload '%s'\r\n", command.c_str(), payload.c_str());
      keyboard_ble_executeCommand(command, payload);
      break;
    }
    #endif

    case SPECIAL: {

      if (command == ALLDEVICES_POWER_TOGGLE) {
        // this variant toggles power state, dependant on the value of allDevsPowered
        // currently we do not toggle, but switch all devices off
        // if (allDevsPowered == 0) {
        //   executeCommand(SAMSUNG_POWER_ON);
        //   delay(500);
        //   executeCommand(YAMAHA_POWER_ON);
        //   delay(4500);
        //   currentScene = "";
        //   allDevsPowered = 1;
        // } else {
          executeCommand(SAMSUNG_POWER_OFF);
          delay(500);
          executeCommand(YAMAHA_POWER_OFF);
          delay(500);
          // you cannot power off FireTV, but at least you can stop the currently running app
          executeCommand(KEYBOARD_HOME);
          delay(500);
          executeCommand(KEYBOARD_HOME);
          currentScene = "";
          allDevsPowered = 0;
        // }

      } else if (command == SCENE_TV) {
        executeCommand(SAMSUNG_POWER_ON);
        delay(500);
        executeCommand(YAMAHA_POWER_ON);
        delay(1500);
        executeCommand(YAMAHA_INPUT_DVD);
        delay(3000);
        executeCommand(SAMSUNG_INPUT_TV);
        
        currentScene = SCENE_TV;
        allDevsPowered = 1;

      } else if (command == SCENE_FIRETV) {
        executeCommand(SAMSUNG_POWER_ON);
        delay(500);
        executeCommand(YAMAHA_POWER_ON);
        delay(1500);
        executeCommand(YAMAHA_INPUT_DTV);
        delay(3000);
        executeCommand(SAMSUNG_INPUT_HDMI_2);
        delay(100);
        
        executeCommand(KEYBOARD_HOME);
        delay(500);
        executeCommand(KEYBOARD_HOME);
        
        currentScene = SCENE_FIRETV;
        allDevsPowered = 1;

      } else if (command == SCENE_CHROMECAST) {
        executeCommand(SAMSUNG_POWER_ON);
        delay(500);
        executeCommand(YAMAHA_POWER_ON);
        delay(1500);
        executeCommand(YAMAHA_INPUT_DVD);
        delay(3000);
        executeCommand(SAMSUNG_INPUT_HDMI_1);
        
        currentScene = SCENE_CHROMECAST;
        allDevsPowered = 1;

      }
      break;
    }
  }
}

void executeCommand(std::string command, std::string additionalPayload) {
  if (commands.count(command) > 0) {
    Serial.printf("command: will execute command '%s'\r\n", command.c_str());
    executeCommandWithData(command, commands[command], additionalPayload);
  } else {
    Serial.printf("command: command '%s' not found\r\n", command.c_str());
  }
}
