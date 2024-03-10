#include "applicationInternal/commandHandler.h"
#include "device_specialCommands.h"

uint16_t COMMAND_UNKNOWN   ;
uint16_t MY_SPECIAL_COMMAND; //"My_special_command";

void register_specialCommands() {
  get_uniqueCommandID(&COMMAND_UNKNOWN);

  // put SPECIAL commands here if you want
  register_command(&MY_SPECIAL_COMMAND , makeCommandData(SPECIAL, {""}));

}
