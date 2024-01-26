#include <Arduino.h>
#include "driver/ledc.h"
#include "hardware/tft.h"
#include "hardware/sleep.h"

TFT_eSPI tft = TFT_eSPI();
Adafruit_FT6206 touch = Adafruit_FT6206();
TS_Point touchPoint;
TS_Point oldPoint;
byte backlight_brightness = 255;

void init_tft(void) {

  // LCD Pin Definition
  pinMode(LCD_EN, OUTPUT);
  digitalWrite(LCD_EN, HIGH);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  // Configure the backlight PWM
  // Manual setup because ledcSetup() briefly turns on the backlight
  ledc_channel_config_t ledc_channel_left;
  ledc_channel_left.gpio_num = (gpio_num_t)LCD_BL;
  ledc_channel_left.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_channel_left.channel = LEDC_CHANNEL_5;
  ledc_channel_left.intr_type = LEDC_INTR_DISABLE;
  ledc_channel_left.timer_sel = LEDC_TIMER_1;
  ledc_channel_left.flags.output_invert = 1; // Can't do this with ledcSetup()
  ledc_channel_left.duty = 0;
	
  ledc_timer_config_t ledc_timer;
  ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_timer.duty_resolution = LEDC_TIMER_8_BIT;
  ledc_timer.timer_num = LEDC_TIMER_1;
  ledc_timer.freq_hz = 640;

  ledc_channel_config(&ledc_channel_left);
  ledc_timer_config(&ledc_timer);

  // Setup TFT 

  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!
  for(int i = 0; i < 100; i++){
    digitalWrite(LCD_EN, HIGH);  // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(LCD_EN, LOW);  // LCD Logic on
  }  

  delay(100); // Wait for the LCD driver to power on
  tft.init();
  tft.initDMA();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  
  // Setup touchscreen
  Wire.begin(SDA, SCL, 400000); // Configure i2c pins and set frequency to 400kHz
  touch.begin(128); // Initialize touchscreen and set sensitivity threshold
}

void update_backligthBrighness(void) {
  static int fadeInTimer = millis(); // fadeInTimer = time after setup
  if(millis() < fadeInTimer + backlight_brightness){ // Fade in the backlight brightness
    ledcWrite(5, millis()-fadeInTimer);
  }
  else { // Dim Backlight before entering standby    
    if(standbyTimer < 2000) ledcWrite(5, 85); // Backlight dim
    else ledcWrite(5, backlight_brightness);  // Backlight on 
  }
}
