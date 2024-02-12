#include "commandHandler.h"
#include "device_smarthome/device_smarthome.h"

void register_device_smarthome() {
  #ifdef ENABLE_KEYBOARD_MQTT
  commands[SMARTHOME_MQTT_BULB1_SET]            = makeCommandData(MQTT, {"bulb1_set"             }); // payload must be set when calling commandHandler
  commands[SMARTHOME_MQTT_BULB2_SET]            = makeCommandData(MQTT, {"bulb2_set"             }); // payload must be set when calling commandHandler
  commands[SMARTHOME_MQTT_BULB1_BRIGHTNESS_SET] = makeCommandData(MQTT, {"bulb1_setbrightness"   }); // payload must be set when calling commandHandler
  commands[SMARTHOME_MQTT_BULB2_BRIGHTNESS_SET] = makeCommandData(MQTT, {"bulb2_setbrightness"   }); // payload must be set when calling commandHandler
  #endif
}
