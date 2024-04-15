#include <Arduino.h>
#include <string>
#include <list>
#include <sstream>
#include <IRremoteESP8266.h>
#include <IRsend.h>

uint8_t IR_LED_GPIO = 33; // IR LED output

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
  switch (protocol) {
    case IR_PROTOCOL_GC: {
      auto current = commandPayloads.begin();
      std::string arrayStr = *current;
      // first create array of needed size
      std::string::difference_type size = std::count(arrayStr.begin(), arrayStr.end(), ',');
      size += 1;
      uint16_t *buf = new uint16_t[size];
      // now get comma separated values and fill array
      int pos = 0;
      std::stringstream ss(arrayStr);
      while(ss.good())  {
        std::string dataStr;
        std::getline(ss, dataStr, ',');
        // https://cplusplus.com/reference/string/stoull/
        std::string::size_type sz = 0;   // alias of size_t
        const uint64_t data = std::stoull(dataStr, &sz, 0);
        // Serial.printf("  next string value %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
        buf[pos] = data;
        pos += 1;
      }
      Serial.printf("execute: will send IR GC, array size %d\r\n", size);
      IrSender.sendGC(buf, size);
      delete [] buf;
      break;
    }

    case IR_PROTOCOL_NEC: {
      auto current = commandPayloads.begin();
      std::string dataStr = *current;
      // https://cplusplus.com/reference/string/stoull/
      std::string::size_type sz = 0;   // alias of size_t
      const uint64_t data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR NEC, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendNEC(data);
      break;
    }

    case IR_PROTOCOL_SAMSUNG: {
      auto current = commandPayloads.begin();
      std::string dataStr = *current;
      // https://cplusplus.com/reference/string/stoull/
      std::string::size_type sz = 0;   // alias of size_t
      const uint64_t data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR SAMSUNG, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendSAMSUNG(data);
      break;
    }

    case IR_PROTOCOL_SONY: {
      std::string::size_type sz = 0;   // alias of size_t
      uint64_t data;
      if (commandPayloads.empty() && (additionalPayload == "")) {
        Serial.printf("execute: cannot send IR SONY, because both data and payload are empty\r\n");
      } else {
        if (additionalPayload != "") {
          data = std::stoull(additionalPayload, &sz, 0);
        } else {
          auto current = commandPayloads.begin();
          data = std::stoull(*current, &sz, 0);
        }
        Serial.printf("execute: will send IR SONY 15 bit, data (%" PRIu64 ")\r\n", data);
        IrSender.sendSony(data, 15);
      }
      break;
    }

    case IR_PROTOCOL_RC5: {
      std::string::size_type sz = 0;   // alias of size_t
      uint64_t data;
      if (commandPayloads.empty() && (additionalPayload == "")) {
        Serial.printf("execute: cannot send IR RC5, because both data and payload are empty\r\n");
      } else {
        if (additionalPayload != "") {
          data = std::stoull(additionalPayload, &sz, 0);
        } else {
          auto current = commandPayloads.begin();
          data = std::stoull(*current, &sz, 0);
        }
        Serial.printf("execute: will send IR RC5, data (%" PRIu64 ")\r\n", data);
        IrSender.sendRC5(IrSender.encodeRC5X(0x00, data));
      }
      break;
    }

    case IR_PROTOCOL_DENON: {
      std::string::size_type sz = 0;   // alias of size_t
      uint64_t data;
      if (commandPayloads.empty() && (additionalPayload == "")) {
        Serial.printf("execute: cannot send IR DENON 48 bit, because both data and payload are empty\r\n");
      } else {
        if (additionalPayload != "") {
          data = std::stoull(additionalPayload, &sz, 0);
        } else {
          auto current = commandPayloads.begin();
          data = std::stoull(*current, &sz, 0);
        }
        Serial.printf("execute: will send IR DENON 48 bit, data (%" PRIu64 ")\r\n", data);
        IrSender.sendDenon(data, 48);
      }
      break;
    }

    case IR_PROTOCOL_SAMSUNG36:
    {
      auto current = commandPayloads.begin();
      std::string dataStr = *current;
      // https://cplusplus.com/reference/string/stoull/
      std::string::size_type sz = 0; // alias of size_t
      const uint64_t data = std::stoull(dataStr, &sz, 0);
      Serial.printf("execute: will send IR SAMSUNG36, data %s (%" PRIu64 ")\r\n", dataStr.c_str(), data);
      IrSender.sendSamsung36(data);
      break;
    }
  }
}