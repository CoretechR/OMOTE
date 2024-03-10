#ifndef __DEVICE_KEYBOARD_MQTT_H__
#define __DEVICE_KEYBOARD_MQTT_H__

// The "MQTT keyboard" simply sends MQTT commands to a remote keyboard, which is connected via USB to a device
// https://github.com/KlausMu/esp32-mqtt-keyboard

#define ENABLE_KEYBOARD_MQTT // Comment out to disable WiFi and MQTT

#ifdef ENABLE_KEYBOARD_MQTT

// if you activate the MQTT keyboard, consider changing the mapping of the keyboard commands to the MQTT keyboard in file "commandHandler.h"

#if defined(ENABLE_KEYBOARD_MQTT) && !(ENABLE_WIFI_AND_MQTT == 1)
static_assert(false, "You have to use \"-D ENABLE_WIFI_AND_MQTT=1\" in \"platformio.ini\" when having \"#define ENABLE_KEYBOARD_MQTT\"");
#endif

#define KEYBOARD_MQTT_UP                   "Keyboard_mqtt_up"
#define KEYBOARD_MQTT_DOWN                 "Keyboard_mqtt_down"
#define KEYBOARD_MQTT_RIGHT                "Keyboard_mqtt_right"
#define KEYBOARD_MQTT_LEFT                 "Keyboard_mqtt_left"
#define KEYBOARD_MQTT_SELECT               "Keyboard_mqtt_select"
#define KEYBOARD_MQTT_SENDSTRING           "Keyboard_mqtt_sendstring"
#define KEYBOARD_MQTT_BACK                 "Keyboard_mqtt_back"
#define KEYBOARD_MQTT_HOME                 "Keyboard_mqtt_home"
#define KEYBOARD_MQTT_MENU                 "Keyboard_mqtt_menu"
#define KEYBOARD_MQTT_SCAN_PREVIOUS_TRACK  "Keyboard_mqtt_scan_previous_track"
#define KEYBOARD_MQTT_REWIND_LONG          "Keyboard_mqtt_rewind_long"
#define KEYBOARD_MQTT_REWIND               "Keyboard_mqtt_rewind"
#define KEYBOARD_MQTT_PLAYPAUSE            "Keyboard_mqtt_playpause"
#define KEYBOARD_MQTT_FASTFORWARD          "Keyboard_mqtt_fastforward"
#define KEYBOARD_MQTT_FASTFORWARD_LONG     "Keyboard_mqtt_fastforward_long"
#define KEYBOARD_MQTT_SCAN_NEXT_TRACK      "Keyboard_mqtt_scan_next_track"
#define KEYBOARD_MQTT_MUTE                 "Keyboard_mqtt_mute"
#define KEYBOARD_MQTT_VOLUME_INCREMENT     "Keyboard_mqtt_volume_increment"
#define KEYBOARD_MQTT_VOLUME_DECREMENT     "Keyboard_mqtt_volume_decrement"

void register_device_keyboard_mqtt();

#endif

#endif /*__DEVICE_KEYBOARD_MQTT_H__*/
