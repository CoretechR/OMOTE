#pragma once
#include "SparkFunLIS3DH.h"

#include "HardwareAbstractionInterface.h"
#include "lvgl.h"
#include <Adafruit_FT6206.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <functional>
#include <memory>

#include "omoteconfig.h"

class HardwareRevX : public HardwareAbstractionInterface {
public:
  static std::shared_ptr<HardwareRevX> getInstance() {
    if (!mInstance) {
      mInstance = std::make_shared<HardwareRevX>();
    }
    return mInstance;
  }

  HardwareRevX() : HardwareAbstractionInterface(){};

  virtual void debugPrint(std::string aDebugMessage) override {}

  virtual void sendIR() override {}

  virtual void MQTTPublish(const char *topic, const char *payload) override {}

  virtual void init();

  void handleLoop();

public:
  virtual void initLVGL();

  void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area,
                    lv_color_t *color_p);
  void touchPadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

  void activityDetection();

  void initIO();

public:
  static void displayFlushImpl(lv_disp_drv_t *disp, const lv_area_t *area,
                               lv_color_t *color_p) {
    mInstance->displayFlush(disp, area, color_p);
  }
  static void touchPadReadImpl(lv_indev_drv_t *indev_driver,
                               lv_indev_data_t *data) {
    mInstance->touchPadRead(indev_driver, data);
  }

  int standbyTimer = SLEEP_TIMEOUT;

  Adafruit_FT6206 touch = Adafruit_FT6206();
  TS_Point touchPoint;
  TS_Point oldPoint;

  TFT_eSPI tft = TFT_eSPI();

  // IMU Motion Detection
  LIS3DH IMU = LIS3DH(I2C_MODE, 0x19); // Default constructor is I2C, addr 0x19.
  bool wakeupByIMUEnabled = true;
  int motion = 0;

  // LVGL Screen Buffers
  lv_disp_draw_buf_t mdraw_buf;
  lv_color_t mbufA[SCREEN_WIDTH * SCREEN_HEIGHT / 10];
  lv_color_t mbufB[SCREEN_WIDTH * SCREEN_HEIGHT / 10];

  static std::shared_ptr<HardwareRevX> mInstance;
};