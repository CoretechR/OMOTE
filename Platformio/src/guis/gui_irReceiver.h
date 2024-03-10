#pragma once

#include <lvgl.h>
#include <string>

const char * const tabName_irReceiver = "IR Receiver";
void register_gui_irReceiver(void);

void showNewIRmessage_cb(std::string);
