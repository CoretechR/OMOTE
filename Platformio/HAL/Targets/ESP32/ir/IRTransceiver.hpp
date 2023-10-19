#pragma once
#include "IRInterface.hpp"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

class IRTransceiver : public IRInterface, protected IRsend, protected IRrecv {
public:
  IRTransceiver();
  virtual ~IRTransceiver();

  virtual void send(int64SendTypes protocol, uint64_t data) override;
  virtual void send(constInt64SendTypes protocol, const uint64_t data) override;
  virtual void send(charArrSendType protocol,
                    const unsigned char data[]) override;

  virtual void enableRx() override;
  virtual void disableRx() override;
  virtual void loopHandleRx() override;

private:
  decode_results mCurrentResults;
};