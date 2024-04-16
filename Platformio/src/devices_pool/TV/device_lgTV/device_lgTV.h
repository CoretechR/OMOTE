#pragma once

// Only activate the commands that are used. Every command takes 100 bytes, wether used or not.
extern uint16_t LGTV_POWER_TOGGLE;
extern uint16_t LGTV_SOURCE;
extern uint16_t LGTV_NUM_1;
extern uint16_t LGTV_NUM_2;
extern uint16_t LGTV_NUM_3;
extern uint16_t LGTV_NUM_4;
extern uint16_t LGTV_NUM_5;
extern uint16_t LGTV_NUM_6;
extern uint16_t LGTV_NUM_7;
extern uint16_t LGTV_NUM_8;
extern uint16_t LGTV_NUM_9;
extern uint16_t LGTV_NUM_0;
extern uint16_t LGTV_PRECH;
extern uint16_t LGTV_VOL_MINUS;
extern uint16_t LGTV_VOL_PLUS;
extern uint16_t LGTV_MUTE_TOGGLE;
//extern uint16_t LGTV_CHLIST;
extern uint16_t LGTV_CHANNEL_UP;
extern uint16_t LGTV_CHANNEL_DOWN;
extern uint16_t LGTV_MENU; // Q-Menu
//extern uint16_t LGTV_APPS;
extern uint16_t LGTV_GUIDE;
extern uint16_t LGTV_SETUP;
extern uint16_t LGTV_INFO;
extern uint16_t LGTV_UP;
extern uint16_t LGTV_DOWN;
extern uint16_t LGTV_LEFT;
extern uint16_t LGTV_RIGHT;
extern uint16_t LGTV_OK;
extern uint16_t LGTV_RETURN;
extern uint16_t LGTV_EXIT;
extern uint16_t LGTV_KEY_A;
extern uint16_t LGTV_KEY_B;
extern uint16_t LGTV_KEY_C;
extern uint16_t LGTV_KEY_D;
//extern uint16_t LGTV_PSIZE;
//extern uint16_t LGTV_ADSUBT;
//extern uint16_t LGTV_REWIND;
//extern uint16_t LGTV_PAUSE;
//extern uint16_t LGTV_FASTFORWARD;
//extern uint16_t LGTV_RECORD;
//extern uint16_t LGTV_PLAY;
//extern uint16_t LGTV_STOP;
//extern uint16_t LGTV_POWER_OFF;
//extern uint16_t LGTV_POWER_ON;
extern uint16_t LGTV_INPUT_HDMI_1;
extern uint16_t LGTV_INPUT_HDMI_2;
extern uint16_t LGTV_INPUT_HDMI_3;
extern uint16_t LGTV_INPUT_TV;
//extern uint16_t LGTV_INPUT_COMPONENT;
//extern uint16_t LGTV_SLEEP;
extern uint16_t LGTV_3D;
extern uint16_t LGTV_HOME;

void register_device_lgTV();