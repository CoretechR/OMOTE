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

/// @brief Supported IR Protocols.
enum IRprotocols {
  /// @brief Standard Global Cache IR protocol. 
  IR_PROTOCOL_GC = 0,
  /// @brief Standard NEC IR protocol. 
  IR_PROTOCOL_NEC = 1,
  /// @brief Standard Samsung IR protocol.
  IR_PROTOCOL_SAMSUNG = 2,
  /// @brief Sony IR protocol operating with 15 bits not the default 20 bits.
  IR_PROTOCOL_SONY = 3,
  /// @brief Standard RC5 IR protocol.
  IR_PROTOCOL_RC5 = 4,
  /// @brief Denon IR protocol operating with 48 bits not the default 48 bits.
  IR_PROTOCOL_DENON = 5,
  /// @brief Standard SAMSUNG 36 bit IR protocol.
  IR_PROTOCOL_SAMSUNG36 = 6,
};

/// @brief Gets the default IR repeat number.
/// @param protocol Which protocol to get the default for.
/// @return The default to use for the repeat. This is the default for the 
///         relevant IrSender.send api.
uint16_t getProtocolDefaultRepeat(int protocol) {
  switch (protocol) {
    case IR_PROTOCOL_SONY: {
      // Sony protocol by default needs 2 repeats.
      return 2;
    }
    default: {
      return 0;
    }
  }
}

/// @brief Gets the defualt IR bits to send.
/// @param protocol Which protocol to get the default for.
/// @return The default to use for the repeat. This is mostly default for the 
///         relevant IrSender.send api.
uint16_t getProtocolDefaultBits(int protocol) {
  switch (protocol) {
    case IR_PROTOCOL_NEC: {
      return 32;
    }

    case IR_PROTOCOL_SAMSUNG: {
      return 32;
    }

    case IR_PROTOCOL_SONY: {
      // This is actually not the default but it's the default that was 
      // selected for the enum value. The API default is 20.
      return 15;
    }

    case IR_PROTOCOL_RC5: {
      return 13;
    }

    case IR_PROTOCOL_DENON: {
      // This is actually not the default but it's the default that was 
      // selected for the enum value. The API default is 15.
      return 48;
    }

    case IR_PROTOCOL_SAMSUNG36: {
      return 38;
    }

    default: {
      return 0;
    }
  }
}

void sendIRcode_HAL(int protocol, std::list<std::string> commandPayloads, std::string additionalPayload) {

  // first determine if data was provided by commandPayload or by additionalPayload. Only one of these will be used.
  std::string::size_type sz = 0;   // alias of size_t
  std::string dataStr;
  uint64_t data;
  if (commandPayloads.empty() && (additionalPayload == "")) {
    Serial.printf("sendIRcode_HAL: cannot send IR command, because both data and payload are empty.\r\n");
    return;
  } else {
    if (additionalPayload != "") {
      Serial.printf("sendIRcode_HAL: Setting data stream to additional payload.\r\n");
      dataStr = additionalPayload;
    } else {
      auto current = commandPayloads.begin();
      dataStr = *current;
    }
  }
  
  uint16_t repeat = getProtocolDefaultRepeat(protocol);
  uint16_t nbits = getProtocolDefaultBits(protocol);

  if (protocol != IR_PROTOCOL_GC)
  {
    std::string::difference_type elementCount = std::count(dataStr.begin(), dataStr.end(), ':');
    elementCount++;
    std::string valueAsStr;

    if (elementCount == 3)
    {
        std::stringstream dataStrAsStream(dataStr);
  
        std::getline(dataStrAsStream, valueAsStr, ':');
        data = std::stoull(valueAsStr, nullptr, 0);

        std::getline(dataStrAsStream, valueAsStr, ':');
        repeat = std::stoull(valueAsStr, nullptr, 0);

        std::getline(dataStrAsStream, valueAsStr, ':');
        nbits = std::stoull(valueAsStr, nullptr, 0);
    }
    else if (elementCount == 2)
    {
        std::stringstream dataStrAsStream(dataStr);
        
        std::string valueAsStr;
        uint64_t valueAsUint64;

        std::getline(dataStrAsStream, valueAsStr, ':');
        data = std::stoull(valueAsStr, nullptr, 0);

        std::getline(dataStrAsStream, valueAsStr, ':');
        repeat = std::stoull(valueAsStr, nullptr, 0);
      }
      else
      {
        data = std::stoull(dataStr, &sz, 0);
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
      Serial.printf("sendIRcode_HAL: will send IR GC, array size '%d'.\r\n", size);
      IrSender.sendGC(buf, size);
      delete [] buf;
      break;
    }

    case IR_PROTOCOL_NEC: {
      Serial.printf("sendIRcode_HAL: will send IR NEC, command: '%s', data: (%" PRIu64 "), bits: (%" PRIu64 "), repeat: (%" PRIu64 ").\r\n", 
        dataStr.c_str(), data, nbits, repeat);
      IrSender.sendNEC(data, nbits, repeat);
      break;
    }

    case IR_PROTOCOL_SAMSUNG: {
      Serial.printf("sendIRcode_HAL: will send IR SAMSUNG, command: '%s', data: (%" PRIu64 "), bits: (%" PRIu64 "), repeat: (%" PRIu64 ").\r\n", 
        dataStr.c_str(), data, nbits, repeat);
      IrSender.sendSAMSUNG(data, nbits, repeat);
      break;
    }

    case IR_PROTOCOL_SONY: {
      Serial.printf("sendIRcode_HAL: will send IR SONY, command: '%s', data: (%" PRIu64 "), bits: (%" PRIu64 "), repeat: (%" PRIu64 ").\r\n", 
        dataStr.c_str(), data, nbits, repeat);
      IrSender.sendSony(data,  nbits, repeat);
      break;
    }

    case IR_PROTOCOL_RC5: {
      Serial.printf("sendIRcode_HAL: will send IR RC5, command: '%s', data: (%" PRIu64 "), bits: (%" PRIu64 "), repeat: (%" PRIu64 ").\r\n", 
        dataStr.c_str(), data, nbits, repeat);
      IrSender.sendRC5(IrSender.encodeRC5X(0x00, data),  nbits, repeat);
      break;
    }

    case IR_PROTOCOL_DENON: {
      Serial.printf("sendIRcode_HAL: will send IR DENON, command: '%s', data: (%" PRIu64 "), bits: (%" PRIu64 "), repeat: (%" PRIu64 ").\r\n", 
        dataStr.c_str(), data, nbits, repeat);
      IrSender.sendDenon(data, nbits, repeat);
      break;
    }

    case IR_PROTOCOL_SAMSUNG36: {
      data = std::stoull(dataStr, &sz, 0);
      Serial.printf("sendIRcode_HAL: will send IR SAMSUNG36, command: '%s', data: (%" PRIu64 "), bits: (%" PRIu64 "), repeat: (%" PRIu64 ").\r\n", 
        dataStr.c_str(), data, nbits, repeat);
      IrSender.sendSamsung36(data, nbits, repeat);
      break;
    }

    default: {
      Serial.printf("sendIRcode_HAL: Unknwon protocol '%i'.\r\n", protocol);
      return;
    }
  }
}