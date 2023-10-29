#pragma once
#include "IRInterface.hpp"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <freertos/task.h>

class IRTransceiver : public IRInterface, protected IRsend, protected IRrecv {
public:
  IRTransceiver();
  virtual ~IRTransceiver();

  void send(int64SendTypes protocol, uint64_t data) override;
  void send(constInt64SendTypes protocol, const uint64_t data) override;
  void send(charArrSendType protocol, const unsigned char data[]) override;
  void send(IRInterface::RawIR aRawIr) override;

  int8_t calibrateTx() override {
    maxOutTaskPriority();
    auto calibrationOffset = IRsend::calibrate();
    restoreTaskPriority();
    Serial.printf("Calibration Offset: %i", calibrationOffset);
    return calibrationOffset;
  };

  void enableRx() override;
  void disableRx() override;
  void loopHandleRx() override;

private:
  void maxOutTaskPriority();
  void restoreTaskPriority();
  BaseType_t mPreSendPriority = 0;

  bool mIsRxEnabled = false;
  decode_results mCurrentResults;
};