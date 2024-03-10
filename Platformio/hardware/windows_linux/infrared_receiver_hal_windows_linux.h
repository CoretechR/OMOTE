#pragma once

#include <string>

void start_infraredReceiver_HAL(void);
void shutdown_infraredReceiver_HAL(void);
void infraredReceiver_loop_HAL(void);

bool get_irReceiverEnabled_HAL();
void set_irReceiverEnabled_HAL(bool aIrReceiverEnabled);

typedef void (*tShowNewIRmessage_cb)(std::string message);
void set_showNewIRmessage_cb_HAL(tShowNewIRmessage_cb pShowNewIRmessage_cb);
