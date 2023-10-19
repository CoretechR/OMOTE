#pragma once
#include "IRInterface.hpp"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

class IRTransceiver : public IRInterface, protected IRsend, protected IRrecv {
public:
  IRTransceiver();
  virtual ~IRTransceiver();

  void send(int64SendTypes protocol, uint64_t data) override;
  void send(constInt64SendTypes protocol, const uint64_t data) override;
  void send(charArrSendType protocol, const unsigned char data[]) override;
  void send(IRInterface::RawIR aRawIr) override;

  void enableRx() override;
  void disableRx() override;
  void loopHandleRx() override;

private:
  decode_results mCurrentResults;
};