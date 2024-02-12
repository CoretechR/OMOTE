#include "commandHandler.h"
#include "device_keyboard_mqtt/device_keyboard_mqtt.h"

#ifdef ENABLE_KEYBOARD_MQTT

void register_device_keyboard_mqtt() {
  commands[KEYBOARD_MQTT_UP]                   = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/UP",                  "PRESS"});
  commands[KEYBOARD_MQTT_DOWN]                 = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/DOWN",                "PRESS"});
  commands[KEYBOARD_MQTT_RIGHT]                = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/RIGHT",               "PRESS"});
  commands[KEYBOARD_MQTT_LEFT]                 = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/LEFT",                "PRESS"});
  commands[KEYBOARD_MQTT_SELECT]               = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/SELECT",              "PRESS"});
  commands[KEYBOARD_MQTT_SENDSTRING]           = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/SENDSTRING"                  }); // payload must be set when calling commandHandler
  commands[KEYBOARD_MQTT_BACK]                 = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/BACK",                "PRESS"});
  commands[KEYBOARD_MQTT_HOME]                 = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/HOME",                "PRESS"});
  commands[KEYBOARD_MQTT_MENU]                 = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/MENU",                "PRESS"});
  commands[KEYBOARD_MQTT_SCAN_PREVIOUS_TRACK]  = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/SCAN_PREVIOUS_TRACK", "PRESS"});
  commands[KEYBOARD_MQTT_REWIND_LONG]          = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/REWIND_LONG",         "PRESS"});
  commands[KEYBOARD_MQTT_REWIND]               = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/REWIND",              "PRESS"});
  commands[KEYBOARD_MQTT_PLAYPAUSE]            = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/PLAYPAUSE",           "PRESS"});
  commands[KEYBOARD_MQTT_FASTFORWARD]          = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/FASTFORWARD",         "PRESS"});
  commands[KEYBOARD_MQTT_FASTFORWARD_LONG]     = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/FASTFORWARD_LONG",    "PRESS"});
  commands[KEYBOARD_MQTT_SCAN_NEXT_TRACK]      = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/SCAN_NEXT_TRACK",     "PRESS"});
  commands[KEYBOARD_MQTT_MUTE]                 = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/MUTE",                "PRESS"});
  commands[KEYBOARD_MQTT_VOLUME_INCREMENT]     = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/VOLUME_INCREMENT",    "PRESS"});
  commands[KEYBOARD_MQTT_VOLUME_DECREMENT]     = makeCommandData(MQTT, {"esp32_keyboard_firetv/cmnd/VOLUME_DECREMENT",    "PRESS"});
}

#endif