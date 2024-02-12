#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "hardware/infrared_sender.h"

IRsend IrSender(IR_LED, true);

void init_infraredSender(void) {
  // IR Pin Definition
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH); // HIGH off - LOW on

  IrSender.begin();
}
