#pragma once

#include <lvgl.h>
#include <string>

const char * const tabName_irReceiver = "IR Receiver";
void register_gui_irReceiver(void);

// used by commandHandler to show WiFi status
void showNewIRmessage(std::string);
void showMQTTmessage(std::string topic, std::string payload);

// used by commandHandler to show ESP-NOW messages
void showEspNowMessage(std::string payload);