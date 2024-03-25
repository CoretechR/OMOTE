#pragma once

#if (ENABLE_KEYBOARD_BLE == 1)

#include <string>
#include <stdint.h>

typedef uint8_t MediaKeyReport[2];
const uint8_t KEY_UP_ARROW = 0;
const uint8_t KEY_DOWN_ARROW = 0;
const uint8_t KEY_RIGHT_ARROW = 0;
const uint8_t KEY_LEFT_ARROW = 0;
const uint8_t KEY_RETURN = 0;
const MediaKeyReport KEY_MEDIA_WWW_BACK = {0, 0};
const MediaKeyReport KEY_MEDIA_WWW_HOME = {0, 0};
const MediaKeyReport KEY_MEDIA_PREVIOUS_TRACK = {0, 0};
const MediaKeyReport KEY_MEDIA_REWIND = {0, 0};
const MediaKeyReport KEY_MEDIA_PLAY_PAUSE = {0, 0};
const MediaKeyReport KEY_MEDIA_FASTFORWARD = {0, 0};
const MediaKeyReport KEY_MEDIA_NEXT_TRACK = {0, 0};
const MediaKeyReport KEY_MEDIA_MUTE = {0, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_UP = {0, 0};
const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {0, 0};

void init_keyboardBLE_HAL();
bool keyboardBLE_isConnected_HAL();
void keyboardBLE_end_HAL();
void keyboardBLE_write_HAL(uint8_t c);
void keyboardBLE_longpress_HAL(uint8_t c);
void keyboardBLE_home_HAL();
void keyboardBLE_sendString_HAL(const std::string &s);
void consumerControlBLE_write_HAL(const MediaKeyReport value);
void consumerControlBLE_longpress_HAL(const MediaKeyReport value);

#endif