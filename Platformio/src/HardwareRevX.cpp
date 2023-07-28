#include "HardwareRevX.hpp"

std::shared_ptr<HardwareRevX> HardwareRevX::mInstance = nullptr;

void HardwareRevX::initIO() {
  // Button Pin Definition
  pinMode(SW_1, OUTPUT);
  pinMode(SW_2, OUTPUT);
  pinMode(SW_3, OUTPUT);
  pinMode(SW_4, OUTPUT);
  pinMode(SW_5, OUTPUT);
  pinMode(SW_A, INPUT);
  pinMode(SW_B, INPUT);
  pinMode(SW_C, INPUT);
  pinMode(SW_D, INPUT);
  pinMode(SW_E, INPUT);

  // Power Pin Definition
  pinMode(CRG_STAT, INPUT_PULLUP);
  pinMode(ADC_BAT, INPUT);

  // IR Pin Definition
  pinMode(IR_RX, INPUT);
  pinMode(IR_LED, OUTPUT);
  pinMode(IR_VCC, OUTPUT);
  digitalWrite(IR_LED, HIGH); // HIGH off - LOW on
  digitalWrite(IR_VCC, LOW);  // HIGH on - LOW off

  // LCD Pin Definition
  pinMode(LCD_EN, OUTPUT);
  digitalWrite(LCD_EN, HIGH);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  // Other Pin Definition
  pinMode(ACC_INT, INPUT);
  pinMode(USER_LED, OUTPUT);
  digitalWrite(USER_LED, LOW);

  // Release GPIO hold in case we are coming out of standby
  gpio_hold_dis((gpio_num_t)SW_1);
  gpio_hold_dis((gpio_num_t)SW_2);
  gpio_hold_dis((gpio_num_t)SW_3);
  gpio_hold_dis((gpio_num_t)SW_4);
  gpio_hold_dis((gpio_num_t)SW_5);
  gpio_hold_dis((gpio_num_t)LCD_EN);
  gpio_hold_dis((gpio_num_t)LCD_BL);
  gpio_deep_sleep_hold_dis();
}

void HardwareRevX::init() {

  // Make sure ESP32 is running at full speed
  setCpuFrequencyMhz(240);

  initIO();

  // Setup TFT
  tft.init();
  tft.initDMA();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  initLVGL();
}

void HardwareRevX::initLVGL() {
  {
    lv_init();

    lv_disp_draw_buf_init(&mdraw_buf, mbufA, mbufB,
                          SCREEN_WIDTH * SCREEN_HEIGHT / 10);

    // Initialize the display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = &HardwareRevX::displayFlushImpl;
    disp_drv.draw_buf = &mdraw_buf;
    lv_disp_drv_register(&disp_drv);

    // Initialize the touchscreen driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = &HardwareRevX::touchPadReadImpl;
    lv_indev_drv_register(&indev_drv);
  }
}

void HardwareRevX::displayFlush(lv_disp_drv_t *disp, const lv_area_t *area,
                                lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixelsDMA((uint16_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void HardwareRevX::touchPadRead(lv_indev_drv_t *indev_driver,
                                lv_indev_data_t *data) {
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

void HardwareRevX::activityDetection() {
  static int accXold;
  static int accYold;
  static int accZold;
  int accX = IMU.readFloatAccelX() * 1000;
  int accY = IMU.readFloatAccelY() * 1000;
  int accZ = IMU.readFloatAccelZ() * 1000;

  // determine motion value as da/dt
  motion = (abs(accXold - accX) + abs(accYold - accY) + abs(accZold - accZ));
  // Calculate time to standby
  standbyTimer -= 100;
  if (standbyTimer < 0)
    standbyTimer = 0;
  // If the motion exceeds the threshold, the standbyTimer is reset
  if (motion > MOTION_THRESHOLD)
    standbyTimer = SLEEP_TIMEOUT;

  // Store the current acceleration and time
  accXold = accX;
  accYold = accY;
  accZold = accZ;
}
