
#include "display.hpp"
#include "omoteconfig.h"
#include "Wire.h"

std::shared_ptr<Display> Display::getInstance(int& standby_timer)
{
    if (DisplayInterface::mInstance == nullptr)
    {
        DisplayInterface::mInstance  = std::shared_ptr<Display>(new Display(LCD_EN, LCD_BL, standby_timer));
    }
    return std::static_pointer_cast<Display>(mInstance);
}

Display::Display(int backlight_pin, int enable_pin, int& standby_timer): DisplayInterface(),
    mBacklightPin(backlight_pin),
    mEnablePin(enable_pin),
    tft(TFT_eSPI()),
    touch(Adafruit_FT6206()),
    standbyTimer(standby_timer)
{
    pinMode(mEnablePin, OUTPUT);
    digitalWrite(mEnablePin, HIGH);
    pinMode(mBacklightPin, OUTPUT);
    digitalWrite(mBacklightPin, HIGH);

    ledcSetup(LCD_BACKLIGHT_LEDC_CHANNEL, LCD_BACKLIGHT_LEDC_FREQUENCY, LCD_BACKLIGHT_LEDC_BIT_RESOLUTION);
    ledcAttachPin(mBacklightPin, LCD_BACKLIGHT_LEDC_CHANNEL);
    ledcWrite(LCD_BACKLIGHT_LEDC_CHANNEL, 0);

    setupTFT();

    // Slowly charge the VSW voltage to prevent a brownout
    // Workaround for hardware rev 1!
    for(int i = 0; i < 100; i++){
        digitalWrite(this->mEnablePin, HIGH);  // LCD Logic off
        delayMicroseconds(1);
        digitalWrite(this->mEnablePin, LOW);  // LCD Logic on
    }  

    setupTouchScreen();
}

void Display::setupTFT() {
  tft.init();
  tft.initDMA();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
}

void Display::setupTouchScreen(){
    // Configure i2c pins and set frequency to 400kHz
    Wire.begin(SDA, SCL, 400000);
    touch.begin(128); // Initialize touchscreen and set sensitivity threshold
}

void Display::setBrightness(uint8_t brigthness)
{
    ledcWrite(LCD_BACKLIGHT_LEDC_CHANNEL, brigthness);
}

void Display::turnOff()
{
    digitalWrite(this->mBacklightPin, HIGH);
    digitalWrite(this->mEnablePin, HIGH);
    pinMode(this->mBacklightPin, INPUT);
    pinMode(this->mEnablePin, INPUT);
    gpio_hold_en((gpio_num_t) mBacklightPin);
    gpio_hold_en((gpio_num_t) mEnablePin);
}

void Display::screenInput(lv_indev_drv_t *indev_driver, lv_indev_data_t *data){
 // int16_t touchX, touchY;
  touchPoint = touch.getPoint();
  int16_t touchX = touchPoint.x;
  int16_t touchY = touchPoint.y;
  bool touched = false;
  if ((touchX > 0) || (touchY > 0)) {
    touched = true;
    standbyTimer = SLEEP_TIMEOUT;
  }

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    // Set the coordinates
    data->point.x = SCREEN_WIDTH - touchX;
    data->point.y = SCREEN_HEIGHT - touchY;

    // Serial.print( "touchpoint: x" );
    // Serial.print( touchX );
    // Serial.print( " y" );
    // Serial.println( touchY );
    // tft.drawFastHLine(0, screenHeight - touchY, screenWidth, TFT_RED);
    // tft.drawFastVLine(screenWidth - touchX, 0, screenHeight, TFT_RED);
  }
}

void Display::flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixelsDMA((uint16_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}