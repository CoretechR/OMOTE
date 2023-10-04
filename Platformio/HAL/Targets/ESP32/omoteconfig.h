#pragma once

#define IS_SIMULATOR false

// Comment out to disable connected features
#define ENABLE_WIFI
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define MQTT_SERVER "YOUR_MQTT_SERVER_IP"

// time until device enters sleep mode in milliseconds
#define SLEEP_TIMEOUT 20000

// motion above threshold keeps device awake
#define MOTION_THRESHOLD 50

// IO34+IO35+IO37+IO38+IO39(+IO13)
#define BUTTON_PIN_BITMASK 0b1110110000000000000000000010000000000000

// Pin assignment
// -----------------------------------------------------------------------------------------------------------------------

#define LCD_DC 9 // defined in TFT_eSPI User_Setup.h
#define LCD_CS 5
#define LCD_MOSI 23
#define LCD_SCK 18
#define LCD_BL 4
#define LCD_EN 10

#define USER_LED 2

#define SW_1 32 // 1...5: Output
#define SW_2 26
#define SW_3 27
#define SW_4 14
#define SW_5 12
#define SW_A 37 // A...E: Input
#define SW_B 38
#define SW_C 39
#define SW_D 34
#define SW_E 35

#define IR_RX 15   // IR receiver input
#define ADC_BAT 36 // Battery voltage sense input (1/2 divider)
#define IR_VCC 25  // IR receiver power
#define IR_LED 33  // IR LED output

#define TFT_SCL 22
#define TFT_SDA 19
#define ACC_INT 20

#define CRG_STAT 21 // battery charger feedback