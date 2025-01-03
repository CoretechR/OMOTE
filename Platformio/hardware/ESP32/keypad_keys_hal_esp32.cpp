#include <Arduino.h>
#include "driver/ledc.h"
#include "lib/Keypad/src/Keypad.h" // modified for inverted logic
#if(OMOTE_HARDWARE_REV >= 5)
  #include <Adafruit_TCA8418.h>
#endif

#if(OMOTE_HARDWARE_REV >= 5)
uint8_t TCA_INT_GPIO = 8;
uint64_t BUTTON_PIN_BITMASK = 0b0000000000000000000000000000000100000100; //IO02+IO08)

Adafruit_TCA8418 keypad;
byte keyboardBrightness = 255;
#else
uint8_t SW_1_GPIO = 32; // 1...5: Output
uint8_t SW_2_GPIO = 26;
uint8_t SW_3_GPIO = 27;
uint8_t SW_4_GPIO = 14;
uint8_t SW_5_GPIO = 12;
uint8_t SW_A_GPIO = 37; // A...E: Input
uint8_t SW_B_GPIO = 38;
uint8_t SW_C_GPIO = 39;
uint8_t SW_D_GPIO = 34;
uint8_t SW_E_GPIO = 35;
uint64_t BUTTON_PIN_BITMASK = 0b1110110000000000000000000010000000000000; //IO34+IO35+IO37+IO38+IO39(+IO13)
#endif

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
#if(OMOTE_HARDWARE_REV >= 5)

#else
byte rowPins[ROWS] = {SW_A_GPIO, SW_B_GPIO, SW_C_GPIO, SW_D_GPIO, SW_E_GPIO}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {SW_1_GPIO, SW_2_GPIO, SW_3_GPIO, SW_4_GPIO, SW_5_GPIO}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
#endif

void init_keys_HAL(void) {
  #if(OMOTE_HARDWARE_REV >= 5)

  if (!keypad.begin(TCA8418_DEFAULT_ADDR, &Wire)) {
    Serial.println("Keypad not found!");
  }
  keypad.matrix(ROWS, COLS);
  pinMode(TCA_INT_GPIO, INPUT);
  keypad.flush();
  keypad.enableInterrupts();

  ledcSetup(LEDC_CHANNEL_6, 5000, 8);
  ledcAttachPin(46, LEDC_CHANNEL_6);
  ledcWrite(LEDC_CHANNEL_6, 0);

  #else
  // Button Pin Definition
  pinMode(SW_1_GPIO, OUTPUT);
  pinMode(SW_2_GPIO, OUTPUT);
  pinMode(SW_3_GPIO, OUTPUT);
  pinMode(SW_4_GPIO, OUTPUT);
  pinMode(SW_5_GPIO, OUTPUT);
  pinMode(SW_A_GPIO, INPUT);
  pinMode(SW_B_GPIO, INPUT);
  pinMode(SW_C_GPIO, INPUT);
  pinMode(SW_D_GPIO, INPUT);
  pinMode(SW_E_GPIO, INPUT);
  #endif
}

enum keypad_keyStates {IDLE_HAL, PRESSED_HAL, HOLD_HAL, RELEASED_HAL};
struct keypad_key {
	char kchar;
	int kcode;
	keypad_keyStates kstate;
	boolean stateChanged;
};
keypad_key keys[LIST_MAX];

void keys_getKeys_HAL(void* ptr) {
  #if(OMOTE_HARDWARE_REV >= 5)

    // https://github.com/adafruit/Adafruit_TCA8418/blob/main/examples/tca8418_keypad_gpio_interrupt/tca8418_keypad_gpio_interrupt.ino
    int intStat = keypad.readRegister(TCA8418_REG_INT_STAT);
    if (intStat & 0x01) // Byte 0: K_INT (keyboard interrupt)
    {
      // datasheet page 16 - Table 2
      int keyCode = keypad.getEvent();
      if (keyCode & 0x80) Serial.print("PRESS\t ");
      else Serial.print("RELEASE\t ");
      //  map keyCode to GPIO nr.
      keyCode &= 0x7F;

      if (keyCode > 96)  //  GPIO
      {
        // process  gpio
        keyCode -= 97;
        Serial.print(keyCode);
        Serial.println();
      }
      else
      {
        // process  matrix
        keyCode--;
        Serial.print(keyCode / 10);
        Serial.print("\t ");
        Serial.print(keyCode % 10);
        Serial.println();
      }

      //  clear the EVENT IRQ flag
      keypad.writeRegister(TCA8418_REG_INT_STAT, 1);
    }
    if (intStat & 0x02) // Byte 1: GPI_INT (GPIO interrupt)
    {
      //  reading the registers is mandatory to clear IRQ flag
      //  can also be used to find the GPIO changed
      //  as these registers are a bitmap of the gpio pins.
      keypad.readRegister(TCA8418_REG_GPIO_INT_STAT_1);
      keypad.readRegister(TCA8418_REG_GPIO_INT_STAT_2);
      keypad.readRegister(TCA8418_REG_GPIO_INT_STAT_3);
      //  clear GPIO IRQ flag
      keypad.writeRegister(TCA8418_REG_INT_STAT, 2);
    }

    //  check pending events
    int intstat = keypad.readRegister(TCA8418_REG_INT_STAT);





    //for(int i=0; i < LIST_MAX; i++) {
    //  (*(keypad_key*)ptr).kchar        = customKeypad.key[i].kchar;
    //  (*(keypad_key*)ptr).kcode        = customKeypad.key[i].kcode;
    //  (*(keypad_key*)ptr).kstate       = (keypad_keyStates)(customKeypad.key[i].kstate);
    //  (*(keypad_key*)ptr).stateChanged = customKeypad.key[i].stateChanged;
    //  // https://www.geeksforgeeks.org/void-pointer-c-cpp/
    //  ptr = (void *) ((intptr_t)(ptr) + sizeof(keypad_key));
    //}
  #else
    customKeypad.getKeys();

    for(int i=0; i < LIST_MAX; i++) {
      (*(keypad_key*)ptr).kchar        = customKeypad.key[i].kchar;
      (*(keypad_key*)ptr).kcode        = customKeypad.key[i].kcode;
      (*(keypad_key*)ptr).kstate       = (keypad_keyStates)(customKeypad.key[i].kstate);
      (*(keypad_key*)ptr).stateChanged = customKeypad.key[i].stateChanged;
      // https://www.geeksforgeeks.org/void-pointer-c-cpp/
      ptr = (void *) ((intptr_t)(ptr) + sizeof(keypad_key));
    }
    #endif
}

#if(OMOTE_HARDWARE_REV >= 5)
void update_keyboardBrightness_HAL(void) {
  // A variable declared static inside a function is visible only inside that function, exists only once (not created/destroyed for each call) and is permanent. It is in a sense a private global variable.
  static int fadeInTimer = millis(); // fadeInTimer = time after setup
  if (millis() < fadeInTimer + keyboardBrightness) {
    // after boot or wakeup, fade in backlight brightness
    // fade in lasts for <backlightBrightness> ms
    ledcWrite(LEDC_CHANNEL_6, millis() - fadeInTimer);
  }
  else {
    // normal mode, set full backlightBrightness
    ledcWrite(LEDC_CHANNEL_6, keyboardBrightness);
  }
}

uint8_t get_keyboardBrightness_HAL() {
  return keyboardBrightness;
};
void set_keyboardBrightness_HAL(uint8_t aKeyboardBrightness) {
  keyboardBrightness = aKeyboardBrightness;
};
#endif