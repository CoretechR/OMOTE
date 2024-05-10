#include <string>
#include "applicationInternal/gui/guiMemoryOptimizer.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/commandHandler.h"
#include "applicationInternal/omote_log.h"

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

    uint16_t command = get_command_short(gui_memoryOptimizer_getActiveSceneName(), keyChar);
    if (command != COMMAND_UNKNOWN) {
      omote_log_d("key: key '%c', will use command '%u'\r\n", keyChar, command);
      executeCommand(command);
    } else {
      omote_log_w("key: key '%c', but no command defined\r\n", keyChar);
    }
  }
}

void doLongPress(char keyChar, int keyCode){
  uint16_t command = get_command_long(gui_memoryOptimizer_getActiveSceneName(), keyChar);
  if (command != COMMAND_UNKNOWN) {
    omote_log_d("key: key '%c' (long press), will use command '%u'\r\n", keyChar, command);
    executeCommand(command);
  } else {
    omote_log_w("key: key '%c' (long press), but no command defined\r\n", keyChar);
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
      omote_log_v("pressed\r\n");
      
      if ((get_key_repeatMode(gui_memoryOptimizer_getActiveSceneName(), keyChar) == SHORT) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != PRESSED)) {
        omote_log_v("key: PRESSED of SHORT key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);
      
      } else if ((get_key_repeatMode(gui_memoryOptimizer_getActiveSceneName(), keyChar) == SHORT_REPEATED) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != PRESSED)) { // here do not repeat it too early, do the repeat only in HOLD
        omote_log_v("key: PRESSED of SHORT_REPEATED key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);

      }
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = PRESSED;

    } else if (keypad_keys[i].kstate == HOLD) {
      omote_log_v("hold\r\n");
      
      if ((get_key_repeatMode(gui_memoryOptimizer_getActiveSceneName(), keyChar) == SHORTorLONG) && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != HOLD)) {
        omote_log_v("key: HOLD of SHORTorLONG key %c (%d)\r\n", keyChar, keyCode);
        omote_log_v("will set keyIsHold to TRUE for keycode %d\r\n", keyCode);
        keyIsHold[keyCode/ROWS][keyCode%ROWS] = true;
        doLongPress(keyChar, keyCode);
      
      } else if (get_key_repeatMode(gui_memoryOptimizer_getActiveSceneName(), keyChar) == SHORT_REPEATED) { // this is the only case where we do not check the lastKeyState, because here it is intended to repeat the action
        omote_log_v("key: HOLD of SHORT_REPEATED key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);

      }
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = HOLD;

    } else if (keypad_keys[i].kstate == RELEASED) {
      omote_log_v("released\r\n");
      if ((get_key_repeatMode(gui_memoryOptimizer_getActiveSceneName(), keyChar) == SHORTorLONG) && !keyIsHold[keyCode/ROWS][keyCode%ROWS] && (lastKeyState[keyCode/ROWS][keyCode%ROWS] != RELEASED)) {
        omote_log_v("value of keyIsHold for keycode %d is %d\r\n", keyCode, keyIsHold[keyCode/ROWS][keyCode%ROWS]);
        omote_log_v("key: RELEASED of SHORTorLONG key %c (%d)\r\n", keyChar, keyCode);
        doShortPress(keyChar, keyCode);
      }
      omote_log_v("will set keyIsHold to FALSE for keycode %d\r\n", keyCode);
      keyIsHold[keyCode/ROWS][keyCode%ROWS] = false;
      
      omote_log_v("key: press of key %c (%d)\r\n", keyChar, keyCode);
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = RELEASED;

    } else if (keypad_keys[i].kstate == IDLE) {
      omote_log_v("idle\r\n");

      omote_log_v("key: idle of key %c (%d)\r\n", keyChar, keyCode);
      lastKeyState[keyCode/ROWS][keyCode%ROWS] = IDLE;

    }
  }
}
