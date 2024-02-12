#include <Arduino.h>
#include "hardware/user_led.h"

void init_userled(void) {
  pinMode(USER_LED, OUTPUT);
  digitalWrite(USER_LED, LOW);  
}
void update_userled(void) {
  digitalWrite(USER_LED, millis() % 1000 > 500);
}
