// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include <Keypad.h> // modified for inverted logic
#include <Preferences.h>
#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <lvgl.h>
#include "WiFi.h"
#include <Adafruit_FT6206.h>
#include "driver/ledc.h"
#include <PubSubClient.h>
#include "display.hpp"
#include "wifiHandler.hpp"
#include "battery.hpp"

#define ENABLE_WIFI // Comment out to diable connected features

// Pin assignment -----------------------------------------------------------------------------------------------------------------------

#define LCD_DC 9 // defined in TFT_eSPI User_Setup.h
#define LCD_CS 5
#define LCD_MOSI 23
#define LCD_SCK 18
#define LCD_BL 4 
#define LCD_EN 10

#define USER_LED 2

#define SW_1 32 // 1...5: Output
#define SW_2 26
#define SW_3 27
#define SW_4 14
#define SW_5 12
#define SW_A 37 // A...E: Input
#define SW_B 38
#define SW_C 39
#define SW_D 34
#define SW_E 35

#define IR_RX 15 // IR receiver input
#define ADC_BAT 36 // Battery voltage sense input (1/2 divider)
#define IR_VCC 25 // IR receiver power
#define IR_LED 33 // IR LED output

#define SCL 22
#define SDA 19
#define ACC_INT 20

#define CRG_STAT 21 // battery charger feedback

// Variables and Object declarations ------------------------------------------------------------------------------------------------------

// LCD declarations
#define screenWidth 240
#define screenHeight 320
Display display(LCD_BL, LCD_EN, screenWidth, screenHeight);
wifiHandler wifihandler;
Battery battery(ADC_BAT, CRG_STAT);
// Battery declares
int battery_voltage = 0;
int battery_percentage = 100;
bool battery_ischarging = false;

// IMU declarations
int motion = 0;
#define SLEEP_TIMEOUT 20000 // time until device enters sleep mode in milliseconds
#define MOTION_THRESHOLD 50 // motion above threshold keeps device awake
int standbyTimer = SLEEP_TIMEOUT;
bool wakeupByIMUEnabled = true;
LIS3DH IMU(I2C_MODE, 0x19); // Default constructor is I2C, addr 0x19.

Adafruit_FT6206 touch = Adafruit_FT6206();
TS_Point touchPoint;
TS_Point oldPoint;
int backlight_brightness = 255;

// LVGL declarations
LV_IMG_DECLARE(gradientLeft);
LV_IMG_DECLARE(gradientRight);
LV_IMG_DECLARE(appleTvIcon);
LV_IMG_DECLARE(appleDisplayIcon);
LV_IMG_DECLARE(appleBackIcon);
LV_IMG_DECLARE(high_brightness);
LV_IMG_DECLARE(low_brightness);
LV_IMG_DECLARE(lightbulb);
lv_obj_t* panel;

// Keypad declarations
const byte ROWS = 5; //four rows
const byte COLS = 5; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'s','^','-','m','r'}, //  source, channel+, Volume-,   mute, record
  {'i','r','+','k','d'}, //    info,    right, Volume+,     OK,   down
  {'4','v','1','3','2'}, //    blue, channel-,     red, yellow,  green
  {'>','o','b','u','l'}, // forward,      off,    back,     up,   left
  {'?','p','c','<','='}  //       ?,     play,  config, rewind,   stop
};
byte rowPins[ROWS] = {SW_A, SW_B, SW_C, SW_D, SW_E}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {SW_1, SW_2, SW_3, SW_4, SW_5}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
#define BUTTON_PIN_BITMASK 0b1110110000000000000000000010000000000000 //IO34+IO35+IO37+IO38+IO39(+IO13)
byte keyMapTechnisat[ROWS][COLS] = {
  {0x69,0x20,0x11,0x0D,0x56},
  {0x4F,0x37,0x10,0x57,0x51},
  {0x6E,0x21,0x6B,0x6D,0x6C},
  {0x34,0x0C,0x22,0x50,0x55},
  {'?' ,0x35,0x2F,0x32,0x36}
};
byte virtualKeyMapTechnisat[10] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0};
byte currentDevice = 1; // Current Device to control (allows switching mappings between devices)

// IR declarations
IRsend IrSender(IR_LED, true);
IRrecv IrReceiver(IR_RX);

// Other declarations
byte wakeup_reason;
enum Wakeup_reasons{WAKEUP_BY_RESET, WAKEUP_BY_IMU, WAKEUP_BY_KEYPAD};
Preferences preferences;

lv_obj_t* WifiLabel;
WiFiClient espClient;
PubSubClient client(espClient);


// Helper Functions -----------------------------------------------------------------------------------------------------------------------

// Set the page indicator scroll position relative to the tabview scroll position
void store_scroll_value_event_cb(lv_event_t* e){
  float bias = (150.0 + 8.0) / 240.0;
  int offset = 240 / 2 - 150 / 2 - 8 - 50 - 3;
  lv_obj_t* screen = lv_event_get_target(e);
  lv_obj_scroll_to_x(panel, lv_obj_get_scroll_x(screen) * bias - offset, LV_ANIM_OFF);
}

// Update current device when the tabview page is changes
void tabview_device_event_cb(lv_event_t* e){
  currentDevice = lv_tabview_get_tab_act(lv_event_get_target(e));
}


// Virtual Keypad Event handler
void virtualKeypad_event_cb(lv_event_t* e) {
  lv_obj_t* target = lv_event_get_target(e);
  lv_obj_t* cont = lv_event_get_current_target(e);
  if (target == cont) return; // stop if container was clicked
  Serial.println(virtualKeyMapTechnisat[(int)target->user_data]);
  // Send IR command based on the button user data  
  IrSender.sendRC5(IrSender.encodeRC5X(0x00, virtualKeyMapTechnisat[(int)target->user_data]));
}

// Apple Key Event handler
void appleKey_event_cb(lv_event_t* e) {
  // Send IR command based on the event user data  
  IrSender.sendSony(50 + (int)e->user_data, 15);
  Serial.println(50 + (int)e->user_data);
}


// Smart Home Toggle Event handler
void smartHomeToggle_event_cb(lv_event_t * e){
  char payload[8];
  if(lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED)) strcpy(payload,"true");
  else strcpy(payload,"false");
  // Publish an MQTT message based on the event user data  
  if((int)e->user_data == 1) client.publish("bulb1_set", payload);
  if((int)e->user_data == 2) client.publish("bulb2_set", payload);
}

// Smart Home Toggle Event handler
void smartHomeSlider_event_cb(lv_event_t * e){
  lv_obj_t * slider = lv_event_get_target(e);
  char payload[8];
  dtostrf(lv_slider_get_value(slider), 1, 2, payload);
  // Publish an MQTT message based on the event user data
  if((int)e->user_data == 1) client.publish("bulb1_setbrightness", payload);
  if((int)e->user_data == 2) client.publish("bulb2_setbrightness", payload);
}


// Read the touchpad
void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data){
  // int16_t touchX, touchY;
  touchPoint = touch.getPoint();
  int16_t touchX = touchPoint.x;
  int16_t touchY = touchPoint.y;
  bool touched = false;
  if ((touchX > 0) || (touchY > 0)) {
    touched = true;
    standbyTimer = SLEEP_TIMEOUT; 
  }

  if( !touched ){
    data->state = LV_INDEV_STATE_REL;
  }
  else{
    data->state = LV_INDEV_STATE_PR;

    // Set the coordinates
    data->point.x = screenWidth - touchX;
    data->point.y = screenHeight - touchY;

    //Serial.print( "touchpoint: x" );
    //Serial.print( touchX );
    //Serial.print( " y" );
    //Serial.println( touchY );
    //tft.drawFastHLine(0, screenHeight - touchY, screenWidth, TFT_RED);
    //tft.drawFastVLine(screenWidth - touchX, 0, screenHeight, TFT_RED);
  }
}

void activityDetection(){
  static int accXold;
  static int accYold;
  static int accZold;
  int accX = IMU.readFloatAccelX()*1000;
  int accY = IMU.readFloatAccelY()*1000;
  int accZ = IMU.readFloatAccelZ()*1000;

  // determine motion value as da/dt
  motion = (abs(accXold - accX) + abs(accYold - accY) + abs(accZold - accZ));
  // Calculate time to standby
  standbyTimer -= 100;
  if(standbyTimer < 0) standbyTimer = 0;
  // If the motion exceeds the threshold, the standbyTimer is reset
  if(motion > MOTION_THRESHOLD) standbyTimer = SLEEP_TIMEOUT;

  // Store the current acceleration and time 
  accXold = accX;
  accYold = accY;
  accZold = accZ;
}

void configIMUInterrupts()
{
  uint8_t dataToWrite = 0;

  //LIS3DH_INT1_CFG   
  //dataToWrite |= 0x80;//AOI, 0 = OR 1 = AND
  //dataToWrite |= 0x40;//6D, 0 = interrupt source, 1 = 6 direction source
  //Set these to enable individual axes of generation source (or direction)
  // -- high and low are used generically
  dataToWrite |= 0x20;//Z high
  //dataToWrite |= 0x10;//Z low
  dataToWrite |= 0x08;//Y high
  //dataToWrite |= 0x04;//Y low
  dataToWrite |= 0x02;//X high
  //dataToWrite |= 0x01;//X low
  if(wakeupByIMUEnabled) IMU.writeRegister(LIS3DH_INT1_CFG, 0b00101010);
  else IMU.writeRegister(LIS3DH_INT1_CFG, 0b00000000);
  
  //LIS3DH_INT1_THS   
  dataToWrite = 0;
  //Provide 7 bit value, 0x7F always equals max range by accelRange setting
  dataToWrite |= 0x45;
  IMU.writeRegister(LIS3DH_INT1_THS, dataToWrite);
  
  //LIS3DH_INT1_DURATION  
  dataToWrite = 0;
  //minimum duration of the interrupt
  //LSB equals 1/(sample rate)
  dataToWrite |= 0x00; // 1 * 1/50 s = 20ms
  IMU.writeRegister(LIS3DH_INT1_DURATION, dataToWrite);
  
  //LIS3DH_CTRL_REG5
  //Int1 latch interrupt and 4D on  int1 (preserve fifo en)
  IMU.readRegister(&dataToWrite, LIS3DH_CTRL_REG5);
  dataToWrite &= 0xF3; //Clear bits of interest
  dataToWrite |= 0x08; //Latch interrupt (Cleared by reading int1_src)
  //dataToWrite |= 0x04; //Pipe 4D detection from 6D recognition to int1?
  IMU.writeRegister(LIS3DH_CTRL_REG5, dataToWrite);

  //LIS3DH_CTRL_REG3
  //Choose source for pin 1
  dataToWrite = 0;
  //dataToWrite |= 0x80; //Click detect on pin 1
  dataToWrite |= 0x40; //AOI1 event (Generator 1 interrupt on pin 1)
  dataToWrite |= 0x20; //AOI2 event ()
  //dataToWrite |= 0x10; //Data ready
  //dataToWrite |= 0x04; //FIFO watermark
  //dataToWrite |= 0x02; //FIFO overrun
  IMU.writeRegister(LIS3DH_CTRL_REG3, dataToWrite);
  
}

// Enter Sleep Mode
void enterSleep(){
  Serial.println("enterSleep called");

  // Configure IMU
  uint8_t intDataRead;
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC);//clear interrupt
  configIMUInterrupts();
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC);//really clear interrupt

  #ifdef ENABLE_WIFI
  wifihandler.turnOff();
  // Power down modem
  #endif

  display.turnOff();

  // Save settings to internal flash memory
  preferences.putBool("wkpByIMU", wakeupByIMUEnabled);
  preferences.putUChar("currentDevice", currentDevice);
  if(!preferences.getBool("alreadySetUp")) preferences.putBool("alreadySetUp", true);
  preferences.end();
  // Prepare IO states
  digitalWrite(LCD_DC, LOW); // LCD control signals off
  digitalWrite(LCD_CS, LOW);
  digitalWrite(LCD_MOSI, LOW);
  digitalWrite(LCD_SCK, LOW);
  pinMode(CRG_STAT, INPUT); // Disable Pull-Up
  digitalWrite(IR_VCC, LOW); // IR Receiver off

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
  gpio_deep_sleep_hold_en();
  
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  delay(100);
  // Sleep
  esp_deep_sleep_start();
}

// Setup ----------------------------------------------------------------------------------------------------------------------------------

void setup() {  

  setCpuFrequencyMhz(240); // Make sure ESP32 is running at full speed

  // Find out wakeup cause
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1){
    if(log(esp_sleep_get_ext1_wakeup_status())/log(2) == 13) wakeup_reason = WAKEUP_BY_IMU;
    else wakeup_reason = WAKEUP_BY_KEYPAD;
  }
  else {
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

  battery.setup();

  // IR Pin Definition
  pinMode(IR_RX, INPUT);
  pinMode(IR_LED, OUTPUT);
  pinMode(IR_VCC, OUTPUT);
  digitalWrite(IR_LED, HIGH); // HIGH off - LOW on
  digitalWrite(IR_VCC, LOW);  // HIGH on - LOW off

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
  
  Serial.begin(115200);

  // --- Startup ---


  // Restore settings from internal flash memory
  preferences.begin("settings", false);
  if(preferences.getBool("alreadySetUp")){
    wakeupByIMUEnabled = preferences.getBool("wkpByIMU");
    currentDevice = preferences.getUChar("currentDevice");
  }  

  #ifdef ENABLE_WIFI
  wifihandler.begin();
  #endif

  display.setup();
  // Setup TFT 

  // --- LVGL UI Configuration ---  
  display.setup_ui();
  // --- End of LVGL configuration ---


  // Setup IMU
  IMU.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  IMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  IMU.settings.adcEnabled = 0;
  IMU.settings.tempEnabled = 0;
  IMU.settings.xAccelEnabled = 1;
  IMU.settings.yAccelEnabled = 1;
  IMU.settings.zAccelEnabled = 1;
  IMU.begin();  
  uint8_t intDataRead;
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC);//clear interrupt
  
  // Setup IR
  IrSender.begin();
  digitalWrite(IR_VCC, HIGH); // Turn on IR receiver
  IrReceiver.enableIRIn();  // Start the receiver


  lv_timer_handler(); // Run the LVGL UI once before the loop takes over


  Serial.print("Setup finised in ");
  Serial.print(millis());
  Serial.println("ms.");

}

// Loop ------------------------------------------------------------------------------------------------------------------------------------

void loop() { 

  display.update();
  // Blink debug LED at 1 Hz
  digitalWrite(USER_LED, millis() % 1000 > 500);

  // Refresh IMU data at 10Hz
  static unsigned long IMUTaskTimer = millis();
  if(millis() - IMUTaskTimer >= 100){
    activityDetection();
    if(standbyTimer == 0){
      Serial.println("Entering Sleep Mode. Goodbye.");
      enterSleep();
    }
    IMUTaskTimer = millis();
  }

#if 1
  // Update battery stats at 1Hz
  static unsigned long batteryTaskTimer = millis() + 1000; // add 1s to start immediately
  if(millis() - batteryTaskTimer >= 1000){
#if 0
    battery_voltage = analogRead(ADC_BAT)*2*3300/4095 + 350; // 350mV ADC offset
    battery_percentage = constrain(map(battery_voltage, 3700, 4200, 0, 100), 0, 100);
    batteryTaskTimer = millis();
    battery_ischarging = !digitalRead(CRG_STAT);
    // Check if battery is charging, fully charged or disconnected
    if(battery_ischarging || (!battery_ischarging && battery_voltage > 4350)){
      lv_label_set_text(objBattPercentage, "");
      lv_label_set_text(objBattIcon, LV_SYMBOL_USB);
    }
    else{
      // Update status bar battery indicator
      //lv_label_set_text_fmt(objBattPercentage, "%d%%", battery_percentage);
      if(battery_percentage > 95) lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_FULL);
      else if(battery_percentage > 75) lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_3);
      else if(battery_percentage > 50) lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_2);
      else if(battery_percentage > 25) lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_1);
      else lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
    }
#else
    batteryTaskTimer = millis();
    battery.update();
#endif
  }
  #endif

  // Keypad Handling
  customKeypad.getKey(); // Populate key list
  for(int i=0; i<LIST_MAX; i++){ // Handle multiple keys (Not really necessary in this case)
    if(customKeypad.key[i].kstate == PRESSED || customKeypad.key[i].kstate == HOLD){
      standbyTimer = SLEEP_TIMEOUT; // Reset the sleep timer when a button is pressed
      int keyCode = customKeypad.key[i].kcode;
      Serial.println(customKeypad.key[i].kchar);
      // Send IR codes depending on the current device (tabview page)
      if(currentDevice == 1) IrSender.sendRC5(IrSender.encodeRC5X(0x00, keyMapTechnisat[keyCode/ROWS][keyCode%ROWS]));
      else if(currentDevice == 2) IrSender.sendSony((keyCode/ROWS)*(keyCode%ROWS), 15);
    }
  }

  // IR Test
  //tft.drawString("IR Command: ", 10, 90, 1);
  //decode_results results;
  //if (IrReceiver.decode(&results)) {
  //  //tft.drawString(String(results.command) + "        ", 80, 90, 1);
  //  IrReceiver.resume(); // Enable receiving of the next value
  //}
}