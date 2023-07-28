// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include "Wire.h"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>
#include <Keypad.h> // modified for inverted logic
#include <PubSubClient.h>
#include <lvgl.h>

#include "HardwareRevX.hpp"
#include "OmoteUI/OmoteUI.hpp"
#include "omoteconfig.h"

std::shared_ptr<HardwareRevX> hal = nullptr;

int battery_voltage = 0;
int battery_percentage = 100;
bool battery_ischarging = false;

// LCD declarations

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

byte keyMapTechnisat[ROWS][COLS] = {{0x69, 0x20, 0x11, 0x0D, 0x56},
                                    {0x4F, 0x37, 0x10, 0x57, 0x51},
                                    {0x6E, 0x21, 0x6B, 0x6D, 0x6C},
                                    {0x34, 0x0C, 0x22, 0x50, 0x55},
                                    {'?', 0x35, 0x2F, 0x32, 0x36}};
byte virtualKeyMapTechnisat[10] = {0x1, 0x2, 0x3, 0x4, 0x5,
                                   0x6, 0x7, 0x8, 0x9, 0x0};

// IR declarations
IRsend IrSender(IR_LED, true);
IRrecv IrReceiver(IR_RX);

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define MQTT_SERVER "YOUR_MQTT_SERVER_IP"
lv_obj_t *WifiLabel;
WiFiClient espClient;
PubSubClient client(espClient);

// Helper Functions
// -----------------------------------------------------------------------------------------------------------------------

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

  // --- Startup ---

  Serial.begin(115200);

  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!
  for (int i = 0; i < 100; i++) {
    digitalWrite(LCD_EN, HIGH); // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(LCD_EN, LOW); // LCD Logic on
  }

  delay(100); // Wait for the LCD driver to power on

  // Setup touchscreen
  Wire.begin(SDA, SCL,
             400000); // Configure i2c pins and set frequency to 400kHz

  hal = HardwareRevX::getInstance();
  hal->init();

  auto ui = OmoteUI::getInstance(hal);
  ui->layout_UI();

  hal->touch.begin(128); // Initialize touchscreen and set sensitivity threshold
#ifdef ENABLE_WIFI
  // Setup WiFi
  WiFi.setHostname("OMOTE"); // define hostname
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(true);
#endif

  // Setup hal->IMU
  hal->IMU.settings.accelSampleRate =
      50; // Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  hal->IMU.settings.accelRange =
      2; // Max G force readable.  Can be: 2, 4, 8, 16
  hal->IMU.settings.adcEnabled = 0;
  hal->IMU.settings.tempEnabled = 0;
  hal->IMU.settings.xAccelEnabled = 1;
  hal->IMU.settings.yAccelEnabled = 1;
  hal->IMU.settings.zAccelEnabled = 1;
  hal->IMU.begin();
  uint8_t intDataRead;
  hal->IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC); // clear interrupt

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
      fadeInTimer +
          hal->backlight_brightness) { // Fade in the backlight brightness
    ledcWrite(5, millis() - fadeInTimer);
  } else { // Dim Backlight before entering standby
    if (hal->standbyTimer < 2000)
      ledcWrite(5, 85); // Backlight dim
    else
      ledcWrite(5, hal->backlight_brightness); // Backlight on
  }

  // Update LVGL UI
  lv_timer_handler();

  // Blink debug LED at 1 Hz
  digitalWrite(USER_LED, millis() % 1000 > 500);

  // Refresh IMU data at 10Hz
  static unsigned long IMUTaskTimer = millis();
  if (millis() - IMUTaskTimer >= 100) {
    hal->activityDetection();
    if (hal->standbyTimer == 0) {
      Serial.println("Entering Sleep Mode. Goodbye.");
      hal->enterSleep();
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
      hal->standbyTimer =
          SLEEP_TIMEOUT; // Reset the sleep timer when a button is pressed
      int keyCode = customKeypad.key[i].kcode;
      Serial.println(customKeypad.key[i].kchar);
      // Send IR codes depending on the current device (tabview page)
      if (hal->currentDevice == 1)
        IrSender.sendRC5(IrSender.encodeRC5X(
            0x00, keyMapTechnisat[keyCode / ROWS][keyCode % ROWS]));
      else if (hal->currentDevice == 2)
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