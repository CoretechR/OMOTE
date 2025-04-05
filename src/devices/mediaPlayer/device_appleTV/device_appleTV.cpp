#include <string>
#include "applicationInternal/commandHandler.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "device_appleTV.h"

uint16_t APPLETV_UP;
uint16_t APPLETV_DOWN;
uint16_t APPLETV_LEFT;
uint16_t APPLETV_RIGHT;
uint16_t APPLETV_OK;

uint16_t APPLETV_PLAY;
uint16_t APPLETV_PAUSE;

uint16_t APPLETV_10_SECOND_BACK;
uint16_t APPLETV_10_SECOND_FOREWARD;

uint16_t APPLETV_NEXT;
uint16_t APPLETV_PREVIOUS;

uint16_t APPLETV_MENU;
uint16_t APPLETV_HOME;

void register_device_appleTV() {
  register_command(&APPLETV_UP                   , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0x77E15080"}));
  register_command(&APPLETV_DOWN                 , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0x77E13080"}));
  register_command(&APPLETV_LEFT                 , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0x77E19080"}));
  register_command(&APPLETV_RIGHT                , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0x77E16080"}));
  register_command(&APPLETV_OK                   , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0x77E13A80"}));

  register_command(&APPLETV_PLAY                 , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0x77E1FA80"})); 
  register_command(&APPLETV_PAUSE                , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0xA7E14C80:32:1"})); // Code + kNECBits + 1 repeat

  register_command(&APPLETV_10_SECOND_BACK       , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0xA7E16480:32:1"})); // Code + kNECBits + 1 repeat
  register_command(&APPLETV_10_SECOND_FOREWARD   , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0xA7E11080:32:1"})); // Code + kNECBits + 1 repeat

  register_command(&APPLETV_NEXT                 , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0xA7E1C480:32:1"})); // Code + kNECBits + 1 repeat
  register_command(&APPLETV_PREVIOUS             , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0xA7E1A480:32:1"})); // Code + kNECBits + 1 repeat
  
  register_command(&APPLETV_MENU                 , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0x77E1C080"}));
  register_command(&APPLETV_HOME                 , makeCommandData(IR, {std::to_string(IR_PROTOCOL_NEC), "0xA7E10280:32:1"})); // Code + kNECBits + 1 repeat
}
