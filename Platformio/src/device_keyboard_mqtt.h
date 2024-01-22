#ifndef __DEVICE_KEYBOARD_MQTT_H__
#define __DEVICE_KEYBOARD_MQTT_H__

#define ENABLE_KEYBOARD_MQTT // Comment out to diable WiFi and MQTT
#ifdef ENABLE_KEYBOARD_MQTT

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

void init_keyboard_mqtt_commands();

#endif

#endif /*__DEVICE_KEYBOARD_MQTT_H__*/
