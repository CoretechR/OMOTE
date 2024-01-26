#ifndef __INFRARED_H__
#define __INFRARED_H__

#include <IRsend.h>
#include <IRrecv.h>

#define IR_LED 33 // IR LED output

// IR declarations
extern IRsend IrSender;

void init_infrared(void);

#endif /*__INFRARED_H__*/
