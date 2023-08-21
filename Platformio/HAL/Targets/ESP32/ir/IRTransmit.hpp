#include "IRTxInterface.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

class IRTransmit: public IRInterface, public IRsend {

    virtual void send(int64SendTypes protocol, uint64_t data) override;
    virtual void send(constInt64SendTypes protocol, const uint64_t data) override;
    virtual void send(charArrSendType protocol, const unsigned char data[]) override;

}; 