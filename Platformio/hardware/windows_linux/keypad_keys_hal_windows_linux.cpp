#include <stdint.h>

#include "keypad_gui/keypad_gui.h"

void init_keys_HAL(void) {
}

enum keypad_keyStates {IDLE_HAL, PRESSED_HAL, HOLD_HAL, RELEASED_HAL};
struct keypad_key {
	char kchar;
	int kcode;
	keypad_keyStates kstate;
	bool stateChanged;
};
keypad_key keys[10];

void keys_getKeys_HAL(void* ptr) {
  auto keyState = pumpKeys();

  // Populate the single key the keypad gui supports
  (*(keypad_key*)ptr).kchar        = keyState.key.key;
  (*(keypad_key*)ptr).kcode        = keyState.key.keyCode;
  (*(keypad_key*)ptr).kstate       = (keypad_keyStates)(keyState.key.state);
  (*(keypad_key*)ptr).stateChanged = keyState.stateChanged;
  ptr = (void *) ((intptr_t)(ptr) + sizeof(keypad_key));

  // Pad out the rest of the possible keys with empty values
  for(int i=1; i < 10; i++) {
    (*(keypad_key*)ptr).kchar        = ' ';
    (*(keypad_key*)ptr).kcode        = 0;
    (*(keypad_key*)ptr).kstate       = IDLE_HAL;
    (*(keypad_key*)ptr).stateChanged = false;
    // https://www.geeksforgeeks.org/void-pointer-c-cpp/
    ptr = (void *) ((intptr_t)(ptr) + sizeof(keypad_key));
  }
}