#include <string>
#include <list>

void init_infraredSender_HAL(void) {
}

// IR protocols
enum IRprotocols
{
  IR_PROTOCOL_GC = 0,
  IR_PROTOCOL_NEC = 1,
  IR_PROTOCOL_SAMSUNG = 2,
  IR_PROTOCOL_SONY = 3,
  IR_PROTOCOL_RC5 = 4,
  IR_PROTOCOL_DENON = 5,
  IR_PROTOCOL_SAMSUNG36 = 6
};
void sendIRcode_HAL(int protocol, std::list<std::string> commandPayloads, std::string additionalPayload) {
}