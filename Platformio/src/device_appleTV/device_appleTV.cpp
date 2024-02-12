#include "commandHandler.h"
#include "device_appleTV/device_appleTV.h"

void register_device_appleTV() {
  commands[APPLETV_GUI_EVENT_USER_DATA]           = makeCommandData(IR_SONY, {});  // payload must be set when calling commandHandler
}
