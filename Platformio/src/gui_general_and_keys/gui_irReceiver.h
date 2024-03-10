#ifndef __GUI_IRRECEIVER_H__
#define __GUI_IRRECEIVER_H__

#include <lvgl.h>
#include <string>

const char * const tabName_irReceiver = "IR Receiver";
void register_gui_irReceiver(void);
void showNewIRmessage(String);

#endif /*__GUI_IRRECEIVER_H__*/
