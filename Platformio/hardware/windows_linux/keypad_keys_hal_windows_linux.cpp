#include <stdint.h>

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
  for(int i=0; i < 19; i++) {
    (*(keypad_key*)ptr).kchar        = ' ';
    (*(keypad_key*)ptr).kcode        = 0;
    (*(keypad_key*)ptr).kstate       = IDLE_HAL;
    (*(keypad_key*)ptr).stateChanged = false;
    // https://www.geeksforgeeks.org/void-pointer-c-cpp/
    ptr = (void *) ((intptr_t)(ptr) + sizeof(keypad_key));
  }
}