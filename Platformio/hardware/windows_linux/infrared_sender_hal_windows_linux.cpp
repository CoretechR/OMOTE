#include <string>
#include <list>
#include <cstdio>
#include <inttypes.h>

#include <flirc/flirc.h>
#include <ir/ir.h>

#include "util.h"

static int fl_transmit_fake(uint16_t *buf, uint16_t len, uint16_t ik, uint8_t repeat)
{
	int i = 0;
	while (repeat --) {	
		for (i = 0; i < len; i++) {
			i & 0x1 ? printf("-") : printf("+");
			printf("%d ", buf[i]);
		}
		printf("\n");
		printf("-%d\n", ik*1000);
	}
	return 0;
}

void init_infraredSender_HAL(void) {
  printf("%-20s %-10s\n", "Lib Flirc Version: ", fl_lib_version());
	printf("%-20s %-10s\n", "Lib IR Version: ",    ir_lib_version());

  if (fl_open_device_alt(0x20A0, "flirc.tv") < 0) {
		printf("Error: unable to open device\n");
		/**
		 * We can't open the device, so let's fake print the output to
		 * stdout.
		 */
		ir_register_tx(fl_transmit_fake);
	} else {
		/**
		 * Only necessary if you want to transmit. Purpose of libir is to
		 * support any transmitter.
		 */
		ir_register_tx(fl_transmit_raw);
	}
}

// IR protocols
enum IRprotocols {
  IR_PROTOCOL_GC = 0,
  IR_PROTOCOL_NEC = 1,
  IR_PROTOCOL_SAMSUNG = 2,
  IR_PROTOCOL_SONY = 3,
  IR_PROTOCOL_RC5 = 4,
  IR_PROTOCOL_DENON = 5,
  IR_PROTOCOL_SAMSUNG36 = 6
};

void sendIRcode_HAL(int protocol, std::list<std::string> commandPayloads, std::string additionalPayload) {
  // first determine if data was provided by commandPayload or by additionalPayload. Only one of these will be used.
  std::string::size_type sz = 0;   // alias of size_t
  std::string dataStr;
  uint64_t data;
  if (commandPayloads.empty() && (additionalPayload == "")) {
    printf("execute: cannot send IR command, because both data and payload are empty\r\n");
    return;
  } else {
    if (additionalPayload != "") {
      dataStr = additionalPayload;
    } else {
      auto current = commandPayloads.begin();
      dataStr = *current;
    }
  }

  switch (protocol) {
    case IR_PROTOCOL_GC: {
      
      break;
    }

    case IR_PROTOCOL_NEC: {
      data = std::stoull(dataStr, &sz, 0);
      
      printf("execute: will send IR NEC, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);

      uint64_t test = reverseBits(data);

      printf("test: (%" PRIu64 ")\r\n", test);

      ir_tx(RC_PROTO_NEC, test, 0);
      break;
    }

    case IR_PROTOCOL_SAMSUNG: {
      data = std::stoull(dataStr, &sz, 0);
      printf("execute: will send IR SAMSUNG, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      //IrSender.sendSAMSUNG(data);
      break;
    }

    case IR_PROTOCOL_SONY: {
      data = std::stoull(dataStr, &sz, 0);
      printf("execute: will send IR SONY 15 bit, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      ir_tx(RC_PROTO_SONY15, data, 2);
      break;
    }

    case IR_PROTOCOL_RC5: {
      data = std::stoull(dataStr, &sz, 0);
      printf("execute: will send IR RC5, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      // todo?? IrSender.encodeRC5X(0x00, data),
      ir_tx(RC_PROTO_RC5, data, 0);
      break;
    }

    case IR_PROTOCOL_DENON: {
      data = std::stoull(dataStr, &sz, 0);
      printf("execute: will send IR DENON 48 bit, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      //IrSender.sendDenon(data, 48);
      ir_tx(RC_PROTO_DENON, data, 0);
      break;
    }

    case IR_PROTOCOL_SAMSUNG36: {
      data = std::stoull(dataStr, &sz, 0);
      printf("execute: will send IR SAMSUNG36, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      //IrSender.sendSamsung36(data);
      break;
    }
  }

}