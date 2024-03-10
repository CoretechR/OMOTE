#include <string>
#include "applicationInternal/commandHandler.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "device_appleTV.h"

uint16_t APPLETV_GUI_EVENT_USER_DATA; //"AppleTV_gui_event_user_data";

void register_device_appleTV() {
  register_command(&APPLETV_GUI_EVENT_USER_DATA           , makeCommandData(IR, {std::to_string(IR_PROTOCOL_SONY)}));  // payload must be set when calling commandHandler
}
