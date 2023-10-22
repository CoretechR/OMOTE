#include "IRInterface.hpp"

class IRSim : public IRInterface {
  void send(int64SendTypes protocol, uint64_t data);
  void send(constInt64SendTypes protocol, const uint64_t data);
  void send(charArrSendType protocol, const unsigned char data[]);
  void send(RawIR aRawIr);

  void enableRx();
  void disableRx();
  void loopHandleRx();
};