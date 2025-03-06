#include <string>
#include "applicationInternal/commandHandler.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "device_appleTV.h"
#if (ENABLE_HUB_COMMUNICATION == 1)
#include "devices/misc/device_hub_helper.h"
#endif

uint16_t APPLETV_GUI_EVENT_USER_DATA; //"AppleTV_gui_event_user_data";

// Command IDs for Apple TV
uint16_t APPLETV_UP;
uint16_t APPLETV_DOWN;
uint16_t APPLETV_LEFT;
uint16_t APPLETV_RIGHT;
uint16_t APPLETV_SELECT;
// ... other commands

void register_device_appleTV() {
  register_command(&APPLETV_GUI_EVENT_USER_DATA           , makeCommandData(IR, {std::to_string(IR_PROTOCOL_SONY)}));  // payload must be set when calling commandHandler

#if (ENABLE_HUB_COMMUNICATION == 1)
  // Register hub commands for Apple TV
  register_hub_command(&APPLETV_UP, "AppleTV", "up");
  register_hub_command(&APPLETV_DOWN, "AppleTV", "down");
  register_hub_command(&APPLETV_LEFT, "AppleTV", "left");
  register_hub_command(&APPLETV_RIGHT, "AppleTV", "right");
  register_hub_command(&APPLETV_SELECT, "AppleTV", "select");
  
  // Example with additional parameters
  register_hub_command_with_params(
    &APPLETV_VOLUME_UP, 
    "AppleTV", 
    "volume", 
    {"action", "up", "increment", "5"}
  );
#endif
}
