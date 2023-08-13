#include "HardwareRevX.hpp"
#include "wifihandler.hpp"
#include "driver/ledc.h"

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

HardwareRevX::HardwareRevX():
  HardwareAbstract(
    std::make_shared<Battery>(ADC_BAT,CRG_STAT),
    wifiHandler::getInstance()
  ){}

HardwareRevX::WakeReason getWakeReason() {
  // Find out wakeup cause
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1) {
    if (log(esp_sleep_get_ext1_wakeup_status()) / log(2) == 13)
      return HardwareRevX::WakeReason::IMU;
    else
      return HardwareRevX::WakeReason::KEYPAD;
  } else {
    return HardwareRevX::WakeReason::RESET;
  }
}

void HardwareRevX::init() {
  // Make sure ESP32 is running at full speed
  setCpuFrequencyMhz(240);

  wakeup_reason = getWakeReason();
  initIO();
  setupBacklight();
  Serial.begin(115200);
  restorePreferences();
  slowDisplayWakeup();
  setupTFT();
  setupTouchScreen();
  initLVGL();
  setupWifi();
  setupIMU();
  setupIR();

  debugPrint(std::string("Finished Hardware Setup in %d", millis()));
}

void HardwareRevX::debugPrint(std::string aDebugMessage) {
  Serial.print(aDebugMessage.c_str());
}

// void HardwareRevX::MQTTPublish(const char *topic, const char *payload) {
// #ifdef ENABLE_WIFI
//   if (client.connected()) {
//     client.publish(topic, payload);
//   } else {
//     debugPrint("MQTT Client Not Connected When Attempting Publish.");
//   }
// #else
//   debugPrint("Attempting To Publish MQTT with wifi Disabled!");
// #endif
// }

std::shared_ptr<HardwareRevX> HardwareRevX::getInstance(){
  if (!mInstance) {
    mInstance = std::shared_ptr<HardwareRevX>(new HardwareRevX());
  }
  return mInstance;
}

void HardwareRevX::initLVGL() {
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

void HardwareRevX::handleDisplayFlush(lv_disp_drv_t *disp,
                                      const lv_area_t *area,
                                      lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixelsDMA((uint16_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void HardwareRevX::handleTouchPadRead(lv_indev_drv_t *indev_driver,
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

void HardwareRevX::enterSleep() {
  // Save settings to internal flash memory
  preferences.putBool("wkpByIMU", wakeupByIMUEnabled);
  preferences.putUChar("blBrightness", backlight_brightness);
  preferences.putUChar("currentDevice", currentDevice);
  if (!preferences.getBool("alreadySetUp"))
    preferences.putBool("alreadySetUp", true);
  preferences.end();

  // Configure IMU
  uint8_t intDataRead;
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC); // clear interrupt
  configIMUInterrupts();
  IMU.readRegister(&intDataRead,
                   LIS3DH_INT1_SRC); // really clear interrupt

#ifdef ENABLE_WIFI
  // Power down modem
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
#endif

  // Prepare IO states
  digitalWrite(LCD_DC, LOW); // LCD control signals off
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_MOSI, LOW);
  digitalWrite(LCD_SCK, LOW);
  digitalWrite(LCD_EN, HIGH); // LCD logic off
  digitalWrite(LCD_BL, HIGH); // LCD backlight off
  pinMode(CRG_STAT, INPUT);   // Disable Pull-Up
  digitalWrite(IR_VCC, LOW);  // IR Receiver off

  // Configure button matrix for ext1 interrupt
  pinMode(SW_1, OUTPUT);
  pinMode(SW_2, OUTPUT);
  pinMode(SW_3, OUTPUT);
  pinMode(SW_4, OUTPUT);
  pinMode(SW_5, OUTPUT);
  digitalWrite(SW_1, HIGH);
  digitalWrite(SW_2, HIGH);
  digitalWrite(SW_3, HIGH);
  digitalWrite(SW_4, HIGH);
  digitalWrite(SW_5, HIGH);
  gpio_hold_en((gpio_num_t)SW_1);
  gpio_hold_en((gpio_num_t)SW_2);
  gpio_hold_en((gpio_num_t)SW_3);
  gpio_hold_en((gpio_num_t)SW_4);
  gpio_hold_en((gpio_num_t)SW_5);
  // Force display pins to high impedance
  // Without this the display might not wake up from sleep
  pinMode(LCD_BL, INPUT);
  pinMode(LCD_EN, INPUT);
  gpio_hold_en((gpio_num_t)LCD_BL);
  gpio_hold_en((gpio_num_t)LCD_EN);
  gpio_deep_sleep_hold_en();

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  delay(100);
  // Sleep
  esp_deep_sleep_start();
}

void HardwareRevX::configIMUInterrupts() {
  uint8_t dataToWrite = 0;

  // LIS3DH_INT1_CFG
  // dataToWrite |= 0x80;//AOI, 0 = OR 1 = AND
  // dataToWrite |= 0x40;//6D, 0 = interrupt source, 1 = 6 direction source
  // Set these to enable individual axes of generation source (or direction)
  //  -- high and low are used generically
  dataToWrite |= 0x20; // Z high
  // dataToWrite |= 0x10;//Z low
  dataToWrite |= 0x08; // Y high
  // dataToWrite |= 0x04;//Y low
  dataToWrite |= 0x02; // X high
  // dataToWrite |= 0x01;//X low
  if (wakeupByIMUEnabled)
    IMU.writeRegister(LIS3DH_INT1_CFG, 0b00101010);
  else
    IMU.writeRegister(LIS3DH_INT1_CFG, 0b00000000);

  // LIS3DH_INT1_THS
  dataToWrite = 0;
  // Provide 7 bit value, 0x7F always equals max range by accelRange setting
  dataToWrite |= 0x45;
  IMU.writeRegister(LIS3DH_INT1_THS, dataToWrite);

  // LIS3DH_INT1_DURATION
  dataToWrite = 0;
  // minimum duration of the interrupt
  // LSB equals 1/(sample rate)
  dataToWrite |= 0x00; // 1 * 1/50 s = 20ms
  IMU.writeRegister(LIS3DH_INT1_DURATION, dataToWrite);

  // LIS3DH_CTRL_REG5
  // Int1 latch interrupt and 4D on  int1 (preserve fifo en)
  IMU.readRegister(&dataToWrite, LIS3DH_CTRL_REG5);
  dataToWrite &= 0xF3; // Clear bits of interest
  dataToWrite |= 0x08; // Latch interrupt (Cleared by reading int1_src)
  // dataToWrite |= 0x04; //Pipe 4D detection from 6D recognition to int1?
  IMU.writeRegister(LIS3DH_CTRL_REG5, dataToWrite);

  // LIS3DH_CTRL_REG3
  // Choose source for pin 1
  dataToWrite = 0;
  // dataToWrite |= 0x80; //Click detect on pin 1
  dataToWrite |= 0x40; // AOI1 event (Generator 1 interrupt on pin 1)
  dataToWrite |= 0x20; // AOI2 event ()
  // dataToWrite |= 0x10; //Data ready
  // dataToWrite |= 0x04; //FIFO watermark
  // dataToWrite |= 0x02; //FIFO overrun
  IMU.writeRegister(LIS3DH_CTRL_REG3, dataToWrite);
}

void HardwareRevX::setupBacklight() {
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
}

void HardwareRevX::restorePreferences() {
  // Restore settings from internal flash memory
  preferences.begin("settings", false);
  if (preferences.getBool("alreadySetUp")) {
    wakeupByIMUEnabled = preferences.getBool("wkpByIMU");
    backlight_brightness = preferences.getUChar("blBrightness");
    currentDevice = preferences.getUChar("currentDevice");
  }
}

void HardwareRevX::setupTFT() {
  // Setup TFT
  tft.init();
  tft.initDMA();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
}

void HardwareRevX::setupTouchScreen() {
  // Configure i2c pins and set frequency to 400kHz
  Wire.begin(SDA, SCL, 400000);
  touch.begin(128); // Initialize touchscreen and set sensitivity threshold
}

void HardwareRevX::setupIMU() {
  // Setup hal
  IMU.settings.accelSampleRate =
      50; // Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  IMU.settings.accelRange = 2; // Max G force readable.  Can be: 2, 4, 8, 16
  IMU.settings.adcEnabled = 0;
  IMU.settings.tempEnabled = 0;
  IMU.settings.xAccelEnabled = 1;
  IMU.settings.yAccelEnabled = 1;
  IMU.settings.zAccelEnabled = 1;
  IMU.begin();
  uint8_t intDataRead;
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC); // clear interrupt
}

void HardwareRevX::slowDisplayWakeup() {
  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!
  for (int i = 0; i < 100; i++) {
    digitalWrite(LCD_EN, HIGH); // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(LCD_EN, LOW); // LCD Logic on
  }

  delay(100); // Wait for the LCD driver to power on
}

void HardwareRevX::handleWifiEvent(WiFiEvent_t event) {
#ifdef ENABLE_WIFI
  // Serial.printf("[WiFi-event] event: %d\n", event);
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
    client.setServer(MQTT_SERVER, 1883); // MQTT initialization
    client.connect("OMOTE");             // Connect using a client id
  }
  // Set status bar icon based on WiFi status
  // TODO allow UI to register a Handler for these events

  //   if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP ||
  //       event == ARDUINO_EVENT_WIFI_STA_GOT_IP6) {
  //     lv_label_set_text(WifiLabel, LV_SYMBOL_WIFI);
  //   } else {
  //     lv_label_set_text(WifiLabel, "");
  //   }
#endif
}

void HardwareRevX::setupIR() {
  // Setup IR
  IrSender.begin();
  digitalWrite(IR_VCC, HIGH); // Turn on IR receiver
  IrReceiver.enableIRIn();    // Start the receiver
}

void HardwareRevX::setupWifi() {
#ifdef ENABLE_WIFI
  // Setup WiFi
  WiFi.setHostname("OMOTE"); // define hostname
  WiFi.onEvent(wiFiEventImpl);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(true);
#endif
}

void HardwareRevX::startTasks() {
  if (xTaskCreate(&HardwareRevX::updateBatteryTask, "Battery Percent Update",
                  1024, nullptr, 5, &batteryUpdateTskHndl) != pdPASS) {
    debugPrint("ERROR Could not Create Battery Update Task!");
  }
}

void HardwareRevX::updateBatteryTask(void*){
  while(true){
    if(auto status = mInstance->getBatteryStatus(); status.has_value()){
      mInstance->mBatteryNotification.notify(status.value());
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void HardwareRevX::loopHandler() {

  // TODO Move the backlight handling into task that spawns when the backlight
  // setting changes and then gets deleted when the setting is achieved.
  // Update Backlight brightness
  static int fadeInTimer = millis(); // fadeInTimer = time after setup
  if (millis() <
      fadeInTimer + backlight_brightness) { // Fade in the backlight brightness
    ledcWrite(5, millis() - fadeInTimer);
  } else { // Dim Backlight before entering standby
    if (standbyTimer < 2000)
      ledcWrite(5, 85); // Backlight dim
    else
      ledcWrite(5, backlight_brightness); // Backlight on
  }

  // TODO move to debug task
  // Blink debug LED at 1 Hz
  digitalWrite(USER_LED, millis() % 1000 > 500);

  // Refresh IMU data at 10Hz
  static unsigned long IMUTaskTimer = millis();
  if (millis() - IMUTaskTimer >= 100) {
    activityDetection();
    if (standbyTimer == 0) {
      Serial.println("Entering Sleep Mode. Goodbye.");
      enterSleep();
    }
    IMUTaskTimer = millis();
  }

  // TODO Convert to free RTOS task

  // TODO Create batter change notification for UI

  //   if (battery_ischarging || (!battery_ischarging && battery_voltage >
  //   4350)) {
  //     lv_label_set_text(objBattPercentage, "");
  //     lv_label_set_text(objBattIcon, LV_SYMBOL_USB);
  //   } else {
  //     // Update status bar battery indicator
  //     // lv_label_set_text_fmt(objBattPercentage, "%d%%",
  //     battery_percentage); if (battery_percentage > 95)
  //       lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_FULL);
  //     else if (battery_percentage > 75)
  //       lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_3);
  //     else if (battery_percentage > 50)
  //       lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_2);
  //     else if (battery_percentage > 25)
  //       lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_1);
  //     else
  //       lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
  //   }
  // }

  // Keypad Handling
  customKeypad.getKey(); // Populate key list
  for (int i = 0; i < LIST_MAX;
       i++) { // Handle multiple keys (Not really necessary in this case)
    if (customKeypad.key[i].kstate == PRESSED ||
        customKeypad.key[i].kstate == HOLD) {
      standbyTimer =
          SLEEP_TIMEOUT; // Reset the sleep timer when a button is pressed
      int keyCode = customKeypad.key[i].kcode;
      Serial.println(customKeypad.key[i].kchar);
      // Send IR codes depending on the current device (tabview page)
      if (currentDevice == 1) {
        IrSender.sendRC5(IrSender.encodeRC5X(
            0x00, keyMapTechnisat[keyCode / ROWS][keyCode % ROWS]));
      } else if (currentDevice == 2) {
        IrSender.sendSony((keyCode / ROWS) * (keyCode % ROWS), 15);
      }
    }
  }
  // IR Test
  // tft.drawString("IR Command: ", 10, 90, 1);
  // decode_results results;
  // if (IrReceiver.decode(&results)) {
  // IrReceiver.resume(); // Enable receiving of the next value
  //}  //tft.drawString(String(results.command) + "        ", 80, 90, 1);
  //
}