#pragma once
#include "DisplayAbstract.h"
#include "HardwareAbstract.hpp"
#include "Notification.hpp"
#include "driver/ledc.h"
#include <Adafruit_FT6206.h>
#include <TFT_eSPI.h>
#include <memory>

/*LEDC Channel to use for the LCD backlight*/
#define LCD_BACKLIGHT_LEDC_CHANNEL LEDC_CHANNEL_5

#define LCD_BACKLIGHT_LEDC_FREQUENCY 640

#define LCD_BACKLIGHT_LEDC_BIT_RESOLUTION 8

#define DEFAULT_BACKLIGHT_BRIGHTNESS 128

class Display : public DisplayAbstract {
public:
  static std::shared_ptr<Display> getInstance();

  /// @brief Set brightness setting and fade to it
  /// @param brightness
  virtual void setBrightness(uint8_t brightness) override;
  virtual uint8_t getBrightness() override;
  virtual void turnOff() override;

  std::shared_ptr<Notification<TS_Point>> TouchNotification() {
    return mTouchEvent;
  }

  inline void wake() {
    if (isAsleep) {
      isAsleep = false;
      startFade();
    }
  }
  inline void sleep() {
    if (!isAsleep) {
      isAsleep = true;
      startFade();
    }
  }

protected:
  virtual void flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area,
                            lv_color_t *color_p);
  virtual void screenInput(lv_indev_drv_t *indev_driver,
                           lv_indev_data_t *data) override;

  /// @brief Fade toward brightness based on isAwake
  /// @return True - Fade complete
  ///         False - Fade set point not reached
  bool fade();
  /// @brief Start the Fade task
  void startFade();

  /// @brief Set the actual display brightness right now
  /// @param brightness
  void setCurrentBrightness(uint8_t brightness);

private:
  Display(int backlight_pin, int enable_pin);
  void setupTFT();
  void setupTouchScreen();
  void setupBacklight();

  int mEnablePin;
  int mBacklightPin;
  TFT_eSPI tft;

  Adafruit_FT6206 touch;
  TS_Point touchPoint;
  TS_Point oldPoint;
  std::shared_ptr<Notification<TS_Point>> mTouchEvent =
      std::make_shared<Notification<TS_Point>>();

  TaskHandle_t mDisplayFadeTask = nullptr;
  SemaphoreHandle_t mFadeTaskMutex = nullptr;
  static void fadeImpl(void *aBrightness);

  uint8_t mBrightness = 0;        // Current display brightness
  uint8_t mAwakeBrightness = 100; // Current setting for brightness when awake
  bool isAsleep = false;
};