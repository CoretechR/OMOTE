#include <map>
#include <string.h>
#include "gui_general_and_keys/keys.h"
#include "hardware/sleep.h"
#include "hardware/infrared_sender.h"
#include "hardware/mqtt.h"
#include "device_samsungTV/device_samsungTV.h"
#include "device_yamahaAmp/device_yamahaAmp.h"
#include "scenes/scene_allOff.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"
#include "scenes/scene_chromecast.h"
#include "scenes/sceneRegistry.h"
#include "scenes/sceneHandler.h"
#include "commandHandler.h"

const byte ROWS = 5; //five rows
const byte COLS = 5; //five columns

// Keypad declarations
// define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'s','^','-','m','e'}, //  source, channel+, Volume-,   mute, record
  {'i','r','+','k','d'}, //    info,    right, Volume+,     OK,   down
  {'4','v','1','3','2'}, //    blue, channel-,     red, yellow,  green
  {'>','o','b','u','l'}, // forward,      off,    back,     up,   left
  {'?','p','c','<','='}  //       ?,     play,  config, rewind,   stop
};
/*
                                    off o

stop =    rewind <    play p    forward >

    config c                 info i
                   up u
           left l  OK k   right r
                  down d
    back b                   source s

Volume+ +         mute m        channel+ ^
Volume- -         record e      channel- v

red 1     green 2     yellow 3  blue 4
*/
byte rowPins[ROWS] = {SW_A, SW_B, SW_C, SW_D, SW_E}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {SW_1, SW_2, SW_3, SW_4, SW_5}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void init_keys(void) {
  // Button Pin Definition
  pinMode(SW_1, OUTPUT);
  pinMode(SW_2, OUTPUT);
  pinMode(SW_3, OUTPUT);
  pinMode(SW_4, OUTPUT);
  pinMode(SW_5, OUTPUT);
  pinMode(SW_A, INPUT);
  pinMode(SW_B, INPUT);
  pinMode(SW_C, INPUT);
  pinMode(SW_D, INPUT);
  pinMode(SW_E, INPUT);
}

KeyState lastKeyState[ROWS][COLS] = {
  {IDLE,IDLE,IDLE,IDLE,IDLE},
  {IDLE,IDLE,IDLE,IDLE,IDLE},
  {IDLE,IDLE,IDLE,IDLE,IDLE},
  {IDLE,IDLE,IDLE,IDLE,IDLE},
  {IDLE,IDLE,IDLE,IDLE,IDLE},
};
unsigned long lastTimeSent[ROWS][COLS] ={
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
};
bool keyIsHold[ROWS][COLS] = {
  {false,false,false,false,false},
  {false,false,false,false,false},
  {false,false,false,false,false},
  {false,false,false,false,false},
  {false,false,false,false,false}
};
int repeatRate = 125; // in milliseconds

void doShortPress(char keyChar, int keyCode){
  unsigned long currentMillis = millis();
  if ((currentMillis - lastTimeSent[keyCode/ROWS][keyCode%ROWS]) > repeatRate) {
    lastTimeSent[keyCode/ROWS][keyCode%ROWS] = currentMillis;

    std::string command = get_command_short(currentScene, keyChar);
    if (command != COMMAND_UNKNOWN) {
      Serial.printf("key: key '%c', will use command '%s'\r\n", keyChar, command.c_str());
      executeCommand(command);
    } else {
      Serial.printf("key: key '%c', but no command defined\r\n", keyChar);
    }
  }
}

void doLongPress(char keyChar, int keyCode){
  std::string command = get_command_long(currentScene, keyChar);
  if (command != COMMAND_UNKNOWN) {
    Serial.printf("key: key '%c' (long press), will use command '%s'\r\n", keyChar, command.c_str());
    executeCommand(command);
  } else {
    Serial.printf("key: key '%c' (long press), but no command defined\r\n", keyChar);
  }
}

void keypad_loop(void) {
  // we have to ignore the result, because in case of SINGLE_REPEATED we want to send the command again and again, but the keypad would give us only one single HOLD state, not repeatedly
  customKeypad.getKeys();

  for(int i=0; i < LIST_MAX; i++) {
    if (!customKeypad.key[i].stateChanged) {
      // we are not allowed to do this, because of the same reason as above
      // continue;
    } else {
      resetStandbyTimer(); // Reset the sleep timer when a button is pressed
    }
    char keyChar = customKeypad.key[i].kchar;
    int keyCode = customKeypad.key[i].kcode;

    if (customKeypad.key[i].kstate == PRESSED) {
      // Serial.println("pressed");
      
      if ((get_key_repeatMode(currentScene, keyChar) == SHORT) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != PRESSED)) {
        // Serial.printf("key: PRESSED of SHORT key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);
      
      } else if ((get_key_repeatMode(currentScene, keyChar) == SHORT_REPEATED) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != PRESSED)) { // here do not repeat it too early, do the repeat only in HOLD
        // Serial.printf("key: PRESSED of SHORT_REPEATED key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);

      }
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = PRESSED;

    } else if (customKeypad.key[i].kstate == HOLD) {
      // Serial.println("hold");
      
      if ((get_key_repeatMode(currentScene, keyChar) == SHORTorLONG) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != HOLD)) {
        // Serial.printf("key: HOLD of SHORTorLONG key %c (%d)\r\n", keyChar, keyCode);
        // Serial.printf("will set keyIsHold to TRUE for keycode %d\r\n", keyCode);
        keyIsHold[keyCode/ROWS][keyCode%ROWS] = true;
        doLongPress(keyChar, keyCode);
      
      } else if (get_key_repeatMode(currentScene, keyChar) == SHORT_REPEATED) { // this is the only case where we do not check the lastKeyState, because here it is intended to repeat the action
        // Serial.printf("key: HOLD of SHORT_REPEATED key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);

      }
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = HOLD;

    } else if (customKeypad.key[i].kstate == RELEASED) {
      // Serial.println("released");
      if ((get_key_repeatMode(currentScene, keyChar) == SHORTorLONG) && !keyIsHold[keyCode/ROWS][keyCode%ROWS] && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != RELEASED)) {
        // Serial.printf("value of keyIsHold for keycode %d is %d\r\n", keyCode, keyIsHold[keyCode/ROWS][keyCode%ROWS]);
        // Serial.printf("key: RELEASED of SHORTorLONG key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);
      }
      // Serial.printf("will set keyIsHold to FALSE for keycode %d\r\n", keyCode);
      keyIsHold[keyCode/ROWS][keyCode%ROWS] = false;
      
      // Serial.printf("key: press of key %c (%d)\r\n", keyChar, keyCode);
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = RELEASED;

    } else if (customKeypad.key[i].kstate == IDLE) {
      // Serial.println("idle");

      // Serial.printf("key: idle of key %c (%d)\r\n", keyChar, keyCode);
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = IDLE;

    }
  }
}
