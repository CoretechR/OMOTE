#ifndef __INFRARED_RECEIVER_H__
#define __INFRARED_RECEIVER_H__

#define IR_RX 15 // IR receiver input
#define IR_VCC 25 // IR receiver power

extern bool irReceiverEnabled;

void init_infraredReceiver(void);
void shutdown_infraredReceiver(void);
void infraredReceiver_loop(void);

#endif /*__INFRARED_RECEIVER_H__*/
