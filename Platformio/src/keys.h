#ifndef __KEYS_H__
#define __KEYS_H__

#include <Keypad.h> // modified for inverted logic

#define SW_1 32 // 1...5: Output
#define SW_2 26
#define SW_3 27
#define SW_4 14
#define SW_5 12
#define SW_A 37 // A...E: Input
#define SW_B 38
#define SW_C 39
#define SW_D 34
#define SW_E 35

#define BUTTON_PIN_BITMASK 0b1110110000000000000000000010000000000000 //IO34+IO35+IO37+IO38+IO39(+IO13)

void init_keys(void);
void keypad_loop(void);

#endif /*__KEYS_H__*/
