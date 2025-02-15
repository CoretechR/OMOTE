
#include "display.hpp"

#include "Wire.h"
#include "driver/ledc.h"
#include "omoteconfig.h"

std::shared_ptr<Display> Display::getInstance() {
  if (DisplayAbstract::mInstance == nullptr) {
    DisplayAbstract::mInstance =
        std::shared_ptr<Display>(new Display(LCD_BL, LCD_EN));
  }
  return std::static_pointer_cast<Display>(mInstance);
}

Display::Display(int backlight_pin, int enable_pin)
    : DisplayAbstract(),
      mBacklightPin(backlight_pin),
      mEnablePin(enable_pin),
      tft(TFT_eSPI()),
      touch(Adafruit_FT6206()) {
  pinMode(mEnablePin, OUTPUT);
  digitalWrite(mEnablePin, HIGH);
  pinMode(mBacklightPin, OUTPUT);
  digitalWrite(mBacklightPin, HIGH);

  setupBacklight();  // This eliminates the flash of the backlight

  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!

  for (int i = 0; i < 100; i++) {
    digitalWrite(this->mEnablePin, HIGH);  // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(this->mEnablePin, LOW);  // LCD Logic on
  }

  setupTFT();
  setupTouchScreen();
  mFadeTaskMutex = xSemaphoreCreateBinary();
  xSemaphoreGive(mFadeTaskMutex);
}

void Display::wake() {
  if (mIsAsleep) {
    mIsAsleep = false;
    startFade();
  }
}
void Display::sleep() {
  if (!mIsAsleep) {
    mIsAsleep = true;
    startFade();
  }
}

void Display::setupBacklight() {
  // Configure the backlight PWM
  // Manual setup because ledcSetup() briefly turns on the backlight
  ledc_channel_config_t ledc_channel_left;
  ledc_channel_left.gpio_num = (gpio_num_t)mBacklightPin;
  ledc_channel_left.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_channel_left.channel = LEDC_CHANNEL_5;
  ledc_channel_left.intr_type = LEDC_INTR_DISABLE;
  ledc_channel_left.timer_sel = LEDC_TIMER_1;
  ledc_channel_left.flags.output_invert = 1;  // Can't do this with ledcSetup()
  ledc_channel_left.duty = 0;
  ledc_channel_left.hpoint = 0;
  ledc_timer_config_t ledc_timer;
  ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_timer.duty_resolution = LEDC_TIMER_8_BIT;
  ledc_timer.timer_num = LEDC_TIMER_1;
  ledc_timer.clk_cfg = LEDC_AUTO_CLK;
  ledc_timer.freq_hz = 640;
  ledc_channel_config(&ledc_channel_left);
  ledc_timer_config(&ledc_timer);
}

void Display::setupTFT() {
  delay(100);
  tft.init();
  tft.initDMA();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
}

void Display::setupTouchScreen() {
  // Configure i2c pins and set frequency to 400kHz
  Wire.begin(TFT_SDA, TFT_SCL, 400000);
  touch.begin(128);  // Initialize touchscreen and set sensitivity threshold
}

void Display::setBrightness(uint8_t brightness) {
  mAwakeBrightness = brightness;
  Serial.print("Set Brightness:");
  Serial.println(mAwakeBrightness);
  startFade();
}

uint8_t Display::getBrightness() { return mAwakeBrightness; }

void Display::setCurrentBrightness(uint8_t brightness) {
  mBrightness = brightness;
  auto duty = static_cast<int>(mBrightness);
  ledcWrite(LCD_BACKLIGHT_LEDC_CHANNEL, duty);
  // Serial.print("Current Brightness:");
  // Serial.println(mBrightness);
}

void Display::turnOff() {
  digitalWrite(this->mBacklightPin, HIGH);
  digitalWrite(this->mEnablePin, HIGH);
  pinMode(this->mBacklightPin, INPUT);
  pinMode(this->mEnablePin, INPUT);
  gpio_hold_en((gpio_num_t)mBacklightPin);
  gpio_hold_en((gpio_num_t)mEnablePin);
}

void Display::screenInput(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  // int16_t touchX, touchY;
  touchPoint = touch.getPoint();
  int16_t touchX = touchPoint.x;
  int16_t touchY = touchPoint.y;
  bool touched = false;
  if ((touchX > 0) || (touchY > 0)) {
    touched = true;
    mTouchEvent->notify(touchPoint);
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

void Display::fadeImpl(void *) {
  bool fadeDone = false;
  while (!fadeDone) {
    fadeDone = getInstance()->fade();
    vTaskDelay(3 / portTICK_PERIOD_MS);  // 3 miliseconds between steps
    // 0 - 255 will take about .75 seconds to fade up.
  }

  xSemaphoreTake(getInstance()->mFadeTaskMutex, portMAX_DELAY);
  getInstance()->mDisplayFadeTask = nullptr;
  xSemaphoreGive(getInstance()->mFadeTaskMutex);

  vTaskDelete(nullptr);  // Delete Fade Task
}

bool Display::fade() {
  // Early return no fade needed.
  if (mBrightness == mAwakeBrightness || mIsAsleep && mBrightness == 0) {
    return true;
  }

  bool fadeDown = mIsAsleep || mBrightness > mAwakeBrightness;
  if (fadeDown) {
    setCurrentBrightness(mBrightness - 1);
    auto setPoint = mIsAsleep ? 0 : mAwakeBrightness;
    return mBrightness == setPoint;
  } else {
    setCurrentBrightness(mBrightness + 1);
    return mBrightness == mAwakeBrightness;
  }
}

void Display::startFade() {
  xSemaphoreTake(mFadeTaskMutex, portMAX_DELAY);
  // Only Create Task if it is needed
  if (mDisplayFadeTask == nullptr) {
    xTaskCreate(&Display::fadeImpl, "Display Fade Task", 1024, nullptr, 5,
                &mDisplayFadeTask);
  }
  xSemaphoreGive(mFadeTaskMutex);
}

void Display::flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area,
                           lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixelsDMA((uint16_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}
