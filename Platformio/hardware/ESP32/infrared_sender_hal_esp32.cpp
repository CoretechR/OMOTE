#include <Arduino.h>
#include <string>
#include <list>
#include <sstream>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#if(OMOTE_HARDWARE_REV >= 5)
const uint8_t IR_LED_GPIO = 5;  // IR LED output
#else
const uint8_t IR_LED_GPIO = 33; // IR LED output
#endif

IRsend IrSender(IR_LED_GPIO, true);

void init_infraredSender_HAL(void) {
  // IR Pin Definition
  pinMode(IR_LED_GPIO, OUTPUT);
  digitalWrite(IR_LED_GPIO, HIGH); // HIGH off - LOW on

  IrSender.begin();
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
    Serial.printf("execute: cannot send IR command, because both data and payload are empty\r\n");
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
      // first create array of needed size
      std::string::difference_type size = std::count(dataStr.begin(), dataStr.end(), ',');
      size += 1;
      uint16_t *buf = new uint16_t[size];
      // now get comma separated values and fill array
      int pos = 0;
      std::stringstream ss(dataStr);
      while(ss.good())  {
        std::string valueStr;
        std::getline(ss, valueStr, ',');
        // https://cplusplus.com/reference/string/stoull/
        data = std::stoull(valueStr, &sz, 0);
        // Serial.printf("  next string value %s (%" PRIu64 ")\r\n", valueStr.c_str(), data);
        buf[pos] = data;
        pos += 1;
      }
      Serial.printf("execute: will send IR GC, array size %d\r\n", size);
      IrSender.sendGC(buf, size);
      delete [] buf;
      break;
    }

    case IR_PROTOCOL_NEC: {
      data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR NEC, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendNEC(data);
      break;
    }

    case IR_PROTOCOL_SAMSUNG: {
      data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR SAMSUNG, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendSAMSUNG(data);
      break;
    }

    case IR_PROTOCOL_SONY: {
      data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR SONY 15 bit, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendSony(data, 15);
      break;
    }

    case IR_PROTOCOL_RC5: {
      data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR RC5, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendRC5(IrSender.encodeRC5X(0x00, data));
      break;
    }

    case IR_PROTOCOL_DENON: {
      data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR DENON 48 bit, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendDenon(data, 48);
      break;
    }

    case IR_PROTOCOL_SAMSUNG36: {
      data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR SAMSUNG36, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendSamsung36(data);
      break;
    }
  }
}