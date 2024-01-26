#include "commandHandler.h"
#include "device_appleTV/device_appleTV.h"

void init_appleTV_commands() {
  commands[APPLETV_GUI_EVENT_USER_DATA]           = makeCommandData(IR_SONY, {});  // payload must be set when calling commandHandler
}
