#include <string>
#include "devices/misc/device_specialCommands.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/scenes/sceneHandler.h"
#include "applicationInternal/commandHandler.h"
#include "applicationInternal/hardware/hardwarePresenter.h"

const uint8_t ROWS = 5; //five rows
const uint8_t COLS = 5; //five columns
keypad_keyStates lastKeyState[ROWS][COLS] = {
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

    uint16_t command = get_command_short(get_currentScene(), keyChar);
    if (command != COMMAND_UNKNOWN) {
      Serial.printf("key: key '%c', will use command '%u'\r\n", keyChar, command);
      executeCommand(command);
    } else {
      Serial.printf("key: key '%c', but no command defined\r\n", keyChar);
    }
  }
}

void doLongPress(char keyChar, int keyCode){
  uint16_t command = get_command_long(get_currentScene(), keyChar);
  if (command != COMMAND_UNKNOWN) {
    Serial.printf("key: key '%c' (long press), will use command '%u'\r\n", keyChar, command);
    executeCommand(command);
  } else {
    Serial.printf("key: key '%c' (long press), but no command defined\r\n", keyChar);
  }
}

void keypad_loop(void) {
  // we have to ignore the result, because in case of SINGLE_REPEATED we want to send the command again and again, but the keypad would give us only one single HOLD state, not repeatedly
  getKeys(keypad_keys);

  for(int i=0; i < keypad_maxkeys; i++) {
    if (!keypad_keys[i].stateChanged) {
      // we are not allowed to do this, because of the same reason as above
      // continue;
    } else {
      setLastActivityTimestamp(); // Reset the sleep timer when a button is pressed
    }
    char keyChar = keypad_keys[i].kchar;
    int keyCode = keypad_keys[i].kcode;

    if (keypad_keys[i].kstate == PRESSED) {
      // Serial.println("pressed");
      
      if ((get_key_repeatMode(get_currentScene(), keyChar) == SHORT) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != PRESSED)) {
        // Serial.printf("key: PRESSED of SHORT key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);
      
      } else if ((get_key_repeatMode(get_currentScene(), keyChar) == SHORT_REPEATED) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != PRESSED)) { // here do not repeat it too early, do the repeat only in HOLD
        // Serial.printf("key: PRESSED of SHORT_REPEATED key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);

      }
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = PRESSED;

    } else if (keypad_keys[i].kstate == HOLD) {
      // Serial.println("hold");
      
      if ((get_key_repeatMode(get_currentScene(), keyChar) == SHORTorLONG) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != HOLD)) {
        // Serial.printf("key: HOLD of SHORTorLONG key %c (%d)\r\n", keyChar, keyCode);
        // Serial.printf("will set keyIsHold to TRUE for keycode %d\r\n", keyCode);
        keyIsHold[keyCode/ROWS][keyCode%ROWS] = true;
        doLongPress(keyChar, keyCode);
      
      } else if (get_key_repeatMode(get_currentScene(), keyChar) == SHORT_REPEATED) { // this is the only case where we do not check the lastKeyState, because here it is intended to repeat the action
        // Serial.printf("key: HOLD of SHORT_REPEATED key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);

      }
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = HOLD;

    } else if (keypad_keys[i].kstate == RELEASED) {
      // Serial.println("released");
      if ((get_key_repeatMode(get_currentScene(), keyChar) == SHORTorLONG) && !keyIsHold[keyCode/ROWS][keyCode%ROWS] && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != RELEASED)) {
        // Serial.printf("value of keyIsHold for keycode %d is %d\r\n", keyCode, keyIsHold[keyCode/ROWS][keyCode%ROWS]);
        // Serial.printf("key: RELEASED of SHORTorLONG key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);
      }
      // Serial.printf("will set keyIsHold to FALSE for keycode %d\r\n", keyCode);
      keyIsHold[keyCode/ROWS][keyCode%ROWS] = false;
      
      // Serial.printf("key: press of key %c (%d)\r\n", keyChar, keyCode);
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = RELEASED;

    } else if (keypad_keys[i].kstate == IDLE) {
      // Serial.println("idle");

      // Serial.printf("key: idle of key %c (%d)\r\n", keyChar, keyCode);
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = IDLE;

    }
  }
}
