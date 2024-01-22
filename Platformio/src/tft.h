#ifndef __TFT_H__
#define __TFT_H__

#include <TFT_eSPI.h> // Hardware-specific library
#include <Adafruit_FT6206.h>

#define LCD_DC 9 // defined in TFT_eSPI User_Setup.h
#define LCD_CS 5
#define LCD_MOSI 23
#define LCD_SCK 18
#define LCD_BL 4 
#define LCD_EN 10

#define SCL 22
#define SDA 19

// LCD declarations
extern TFT_eSPI tft;
#define screenWidth 240
#define screenHeight 320
extern Adafruit_FT6206 touch;
extern TS_Point touchPoint;
extern byte backlight_brightness;

void init_tft(void);
void update_backligthBrighness(void);

#endif /*__TFT_H__*/
