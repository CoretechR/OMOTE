// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include "SparkFunLIS3DH.h"
#include "WiFi.h"
#include "Wire.h"
#include "driver/ledc.h"
#include <Adafruit_FT6206.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
#include <Keypad.h> // modified for inverted logic
#include <Preferences.h>
#include <PubSubClient.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <lvgl.h>

#include "HardwareRevX.hpp"
#include "OmoteUI/OmoteUI.hpp"
#include "omoteconfig.h"

// Variables and Object declarations
// ------------------------------------------------------------------------------------------------------

// Battery declares
int battery_voltage = 0;
int battery_percentage = 100;
bool battery_ischarging = false;

// IMU declarations
int motion = 0;
#define SLEEP_TIMEOUT                                                          \
  20000 // time until device enters sleep mode in milliseconds
#define MOTION_THRESHOLD 50 // motion above threshold keeps device awake
int standbyTimer = SLEEP_TIMEOUT;
bool wakeupByIMUEnabled = true;
LIS3DH IMU(I2C_MODE, 0x19); // Default constructor is I2C, addr 0x19.

// LCD declarations
TFT_eSPI tft = TFT_eSPI();
#define screenWidth 240
#define screenHeight 320
Adafruit_FT6206 touch = Adafruit_FT6206();
TS_Point touchPoint;
TS_Point oldPoint;
int backlight_brightness = 255;

// LVGL declarations

lv_obj_t *objBattPercentage;
lv_obj_t *objBattIcon;
LV_IMG_DECLARE(gradientLeft);
LV_IMG_DECLARE(gradientRight);
LV_IMG_DECLARE(appleTvIcon);
LV_IMG_DECLARE(appleDisplayIcon);
LV_IMG_DECLARE(appleBackIcon);
LV_IMG_DECLARE(high_brightness);
LV_IMG_DECLARE(low_brightness);
LV_IMG_DECLARE(lightbulb);
lv_obj_t *panel;
lv_color_t color_primary = lv_color_hex(0x303030); // gray

// Keypad declarations
const byte ROWS = 5; // four rows
const byte COLS = 5; // four columns
// define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'s', '^', '-', 'm', 'r'}, //  source, channel+, Volume-,   mute, record
    {'i', 'r', '+', 'k', 'd'}, //    info,    right, Volume+,     OK,   down
    {'4', 'v', '1', '3', '2'}, //    blue, channel-,     red, yellow,  green
    {'>', 'o', 'b', 'u', 'l'}, // forward,      off,    back,     up,   left
    {'?', 'p', 'c', '<', '='}  //       ?,     play,  config, rewind,   stop
};
byte rowPins[ROWS] = {SW_A, SW_B, SW_C, SW_D,
                      SW_E}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {SW_1, SW_2, SW_3, SW_4,
                      SW_5}; // connect to the column pinouts of the keypad
Keypad customKeypad =
    Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
#define BUTTON_PIN_BITMASK                                                     \
  0b1110110000000000000000000010000000000000 // IO34+IO35+IO37+IO38+IO39(+IO13)
byte keyMapTechnisat[ROWS][COLS] = {{0x69, 0x20, 0x11, 0x0D, 0x56},
                                    {0x4F, 0x37, 0x10, 0x57, 0x51},
                                    {0x6E, 0x21, 0x6B, 0x6D, 0x6C},
                                    {0x34, 0x0C, 0x22, 0x50, 0x55},
                                    {'?', 0x35, 0x2F, 0x32, 0x36}};
byte virtualKeyMapTechnisat[10] = {0x1, 0x2, 0x3, 0x4, 0x5,
                                   0x6, 0x7, 0x8, 0x9, 0x0};
byte currentDevice =
    1; // Current Device to control (allows switching mappings between devices)

// IR declarations
IRsend IrSender(IR_LED, true);
IRrecv IrReceiver(IR_RX);

// Other declarations
byte wakeup_reason;
enum Wakeup_reasons { WAKEUP_BY_RESET, WAKEUP_BY_IMU, WAKEUP_BY_KEYPAD };
Preferences preferences;

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define MQTT_SERVER "YOUR_MQTT_SERVER_IP"
lv_obj_t *WifiLabel;
WiFiClient espClient;
PubSubClient client(espClient);

// Helper Functions
// -----------------------------------------------------------------------------------------------------------------------

// Display flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixelsDMA((uint16_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

// Read the touchpad
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
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
    data->point.x = screenWidth - touchX;
    data->point.y = screenHeight - touchY;

    // Serial.print( "touchpoint: x" );
    // Serial.print( touchX );
    // Serial.print( " y" );
    // Serial.println( touchY );
    // tft.drawFastHLine(0, screenHeight - touchY, screenWidth, TFT_RED);
    // tft.drawFastVLine(screenWidth - touchX, 0, screenHeight, TFT_RED);
  }
}

void activityDetection() {
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

void configIMUInterrupts() {
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

// Enter Sleep Mode
void enterSleep() {
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
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC); // really clear interrupt

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

#ifdef ENABLE_WIFI
// WiFi status event
void WiFiEvent(WiFiEvent_t event) {
  // Serial.printf("[WiFi-event] event: %d\n", event);
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
    client.setServer(MQTT_SERVER, 1883); // MQTT initialization
    client.connect("OMOTE");             // Connect using a client id
  }
  // Set status bar icon based on WiFi status
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP ||
      event == ARDUINO_EVENT_WIFI_STA_GOT_IP6) {
    lv_label_set_text(WifiLabel, LV_SYMBOL_WIFI);
  } else {
    lv_label_set_text(WifiLabel, "");
  }
}
#endif

// Setup
// ----------------------------------------------------------------------------------------------------------------------------------

void setup() {

  setCpuFrequencyMhz(240); // Make sure ESP32 is running at full speed

  // Find out wakeup cause
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1) {
    if (log(esp_sleep_get_ext1_wakeup_status()) / log(2) == 13)
      wakeup_reason = WAKEUP_BY_IMU;
    else
      wakeup_reason = WAKEUP_BY_KEYPAD;
  } else {
    wakeup_reason = WAKEUP_BY_RESET;
  }

  // --- IO Initialization ---

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

  // --- Startup ---

  Serial.begin(115200);

  // Restore settings from internal flash memory
  preferences.begin("settings", false);
  if (preferences.getBool("alreadySetUp")) {
    wakeupByIMUEnabled = preferences.getBool("wkpByIMU");
    backlight_brightness = preferences.getUChar("blBrightness");
    currentDevice = preferences.getUChar("currentDevice");
  }

  // Setup TFT

  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!
  for (int i = 0; i < 100; i++) {
    digitalWrite(LCD_EN, HIGH); // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(LCD_EN, LOW); // LCD Logic on
  }

  delay(100); // Wait for the LCD driver to power on
  tft.init();
  tft.initDMA();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  // Setup touchscreen
  Wire.begin(SDA, SCL,
             400000); // Configure i2c pins and set frequency to 400kHz
  touch.begin(128);   // Initialize touchscreen and set sensitivity threshold

  auto hal = std::make_shared<HardwareRevX>();
  hal->initLVGL(my_disp_flush, my_touchpad_read);

  auto ui = std::make_shared<OmoteUI>(hal);
  ui->layout_UI();

#ifdef ENABLE_WIFI
  // Setup WiFi
  WiFi.setHostname("OMOTE"); // define hostname
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(true);
#endif

  // Setup IMU
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

  // Setup IR
  IrSender.begin();
  digitalWrite(IR_VCC, HIGH); // Turn on IR receiver
  IrReceiver.enableIRIn();    // Start the receiver

  lv_timer_handler(); // Run the LVGL UI once before the loop takes over

  Serial.print("Setup finised in ");
  Serial.print(millis());
  Serial.println("ms.");
}

// Loop
// ------------------------------------------------------------------------------------------------------------------------------------

void loop() {

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

  // Update LVGL UI
  lv_timer_handler();

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

  // Update battery stats at 1Hz
  static unsigned long batteryTaskTimer =
      millis() + 1000; // add 1s to start immediately
  if (millis() - batteryTaskTimer >= 1000) {
    battery_voltage =
        analogRead(ADC_BAT) * 2 * 3300 / 4095 + 350; // 350mV ADC offset
    battery_percentage =
        constrain(map(battery_voltage, 3700, 4200, 0, 100), 0, 100);
    batteryTaskTimer = millis();
    battery_ischarging = !digitalRead(CRG_STAT);
    // Check if battery is charging, fully charged or disconnected
    if (battery_ischarging || (!battery_ischarging && battery_voltage > 4350)) {
      lv_label_set_text(objBattPercentage, "");
      lv_label_set_text(objBattIcon, LV_SYMBOL_USB);
    } else {
      // Update status bar battery indicator
      // lv_label_set_text_fmt(objBattPercentage, "%d%%", battery_percentage);
      if (battery_percentage > 95)
        lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_FULL);
      else if (battery_percentage > 75)
        lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_3);
      else if (battery_percentage > 50)
        lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_2);
      else if (battery_percentage > 25)
        lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_1);
      else
        lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
    }
  }

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
      if (currentDevice == 1)
        IrSender.sendRC5(IrSender.encodeRC5X(
            0x00, keyMapTechnisat[keyCode / ROWS][keyCode % ROWS]));
      else if (currentDevice == 2)
        IrSender.sendSony((keyCode / ROWS) * (keyCode % ROWS), 15);
    }
  }

  // IR Test
  // tft.drawString("IR Command: ", 10, 90, 1);
  // decode_results results;
  // if (IrReceiver.decode(&results)) {
  //  //tft.drawString(String(results.command) + "        ", 80, 90, 1);
  //  IrReceiver.resume(); // Enable receiving of the next value
  //}
}