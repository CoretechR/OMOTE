#pragma once
#include <memory>

#include "lvgl.h"
class DisplayAbstract {
 public:
  DisplayAbstract();
  virtual void setBrightness(uint8_t brightness) = 0;
  virtual uint8_t getBrightness() = 0;
  virtual void turnOff() = 0;

 protected:
  // Set this with a getInstance method in the Child Class
  static std::shared_ptr<DisplayAbstract> mInstance;

 private:
  lv_indev_t *mTouchIndev;

  // LVGL Screen Buffers
  lv_disp_t *mDisplay;
  // lv_color16_t mbufA[SCREEN_WIDTH * SCREEN_HEIGHT];
  // lv_color16_t mbufB[SCREEN_WIDTH * SCREEN_HEIGHT];
};
