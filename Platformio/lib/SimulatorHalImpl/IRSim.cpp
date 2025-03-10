#include "IRSim.hpp"

// Ir will basically just be nothing but no ops in sim for now.

void IRSim::send(int64SendTypes protocol, uint64_t data) {}
void IRSim::send(constInt64SendTypes protocol, const uint64_t data) {}
void IRSim::send(charArrSendType protocol, const unsigned char data[]) {}
void IRSim::send(RawIR aRawIr) {}

int8_t IRSim::calibrateTx() { return 0; }

void IRSim::enableRx() {}
void IRSim::disableRx() {}
void IRSim::loopHandleRx() {}