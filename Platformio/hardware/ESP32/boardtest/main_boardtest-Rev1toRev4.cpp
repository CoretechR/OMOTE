// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include <LovyanGFX.hpp>
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
#include "driver/ledc.h"
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
#include "secrets.h"

#define ENABLE_WIFI // Comment out to diable connected features

// Pin assignment -----------------------------------------------------------------------------------------------------------------------

#define LCD_DC 9
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

// Battery declares
int battery_voltage = 0;
int battery_percentage = 100;
bool battery_ischarging = false;
SFE_MAX1704X fuelGauge(MAX1704X_MAX17048);

// IMU declarations
int motion = 0;
#define SLEEP_TIMEOUT 20000 // time until device enters sleep mode in milliseconds
#define MOTION_THRESHOLD 80 // motion above threshold keeps device awake
int standbyTimer = SLEEP_TIMEOUT;
bool wakeupByIMUEnabled = true;
LIS3DH IMU(I2C_MODE, 0x19); // Default constructor is I2C, addr 0x19.

// LCD declarations
#define screenWidth 240
#define screenHeight 320
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9341 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Touch_FT5x06 _touch_instance;

  public:

  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.freq_write = 40000000;
      cfg.freq_read  = 16000000;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = LCD_SCK;
      cfg.pin_mosi = LCD_MOSI;
      cfg.pin_dc   = LCD_DC;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs           = LCD_CS;
      cfg.pin_rst          = -1;
      cfg.pin_busy         = -1;
      cfg.memory_width     = screenWidth;
      cfg.memory_height    = screenHeight; // 162 or 160 or 132
      cfg.panel_width      = screenWidth;
      cfg.panel_height     = screenHeight;
      cfg.offset_rotation  = 2;

      _panel_instance.config(cfg);
    }
    {
      auto cfg = _touch_instance.config();
      cfg.i2c_addr = 0x38;
      cfg.i2c_port = 0;
      cfg.pin_sda = SDA;
      cfg.pin_scl = SCL;
      cfg.freq = 400000;
      cfg.x_min = 0;
      cfg.x_max = screenWidth-1;
      cfg.y_min = 0;
      cfg.y_max = screenHeight-1;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};
static LGFX tft;
int backlight_brightness = 255;

// LVGL declarations
static lv_disp_draw_buf_t draw_buf;
static lv_color_t bufA[ screenWidth * screenHeight / 10 ];
static lv_color_t bufB[ screenWidth * screenHeight / 10 ];
lv_obj_t* objBattPercentage;
lv_obj_t* objBattIcon;
lv_obj_t* panel;
lv_color_t color_primary = lv_color_hex(0x303030); // gray

// Keypad declarations
const byte ROWS = 5; //four rows
const byte COLS = 5; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'s','^','-','m','e'}, //  source, channel+, Volume-,   mute, record
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
lv_obj_t* ChecksTable;
WiFiClient espClient;

// array for checking which keys have already been recognized
char keysWorking[ROWS][COLS] = {
  {'0','0','0','0','0'},
  {'0','0','0','0','0'},
  {'0','0','0','0','0'},
  {'0','0','0','0','0'},
  {'x','0','0','0','0'} 
};

// Helper Functions -----------------------------------------------------------------------------------------------------------------------

// Set the page indicator scroll position relative to the tabview scroll position
static void store_scroll_value_event_cb(lv_event_t* e){
  float bias = (150.0 + 8.0) / 240.0;
  int offset = 240 / 2 - 150 / 2 - 8 - 50 - 3;
  lv_obj_t* screen = lv_event_get_target(e);
  lv_obj_scroll_to_x(panel, lv_obj_get_scroll_x(screen) * bias - offset, LV_ANIM_OFF);
}

// Wakeup by IMU Switch Event handler
static void WakeEnableSetting_event_cb(lv_event_t * e){
  wakeupByIMUEnabled = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
}

// Display flushing
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ){
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  tft.startWrite();
  tft.setAddrWindow( area->x1, area->y1, w, h );
  tft.pushPixelsDMA( ( uint16_t * )&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready( disp );
}

// Read the touchpad
void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = x;
        data->point.y = y;
    } else {
        data->state = LV_INDEV_STATE_REL;
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
  // Save settings to internal flash memory
  preferences.putBool("wkpByIMU", wakeupByIMUEnabled);
  preferences.putUChar("blBrightness", backlight_brightness);
  preferences.putUChar("currentDevice", currentDevice);
  if(!preferences.getBool("alreadySetUp")) preferences.putBool("alreadySetUp", true);
  preferences.end();

  // Configure IMU
  uint8_t intDataRead;
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC);//clear interrupt
  configIMUInterrupts();
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC);//really clear interrupt

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
void WiFiEvent(WiFiEvent_t event){
  //Serial.printf("[WiFi-event] event: %d\n", event);
  if(event == ARDUINO_EVENT_WIFI_STA_GOT_IP){

  }
  // Set status bar icon based on WiFi status
  if(event == ARDUINO_EVENT_WIFI_STA_GOT_IP || event == ARDUINO_EVENT_WIFI_STA_GOT_IP6){
    //lv_label_set_text(WifiLabel, LV_SYMBOL_WIFI);
    Serial.println(" WiFi Connected!");
    lv_table_set_cell_value_fmt(ChecksTable, 2, 1, LV_SYMBOL_OK);
  }
  else{
    //lv_label_set_text(WifiLabel, "");
  }
}
#endif

// Setup ----------------------------------------------------------------------------------------------------------------------------------

int FuelGaugeInitSuccessful;

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

  // Setup TFT 

  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!
  for(int i = 0; i < 100; i++){
    digitalWrite(LCD_EN, HIGH);  // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(LCD_EN, LOW);  // LCD Logic on
  }  

  delay(100); // Wait for the LCD driver to power on
  tft.init();
  tft.initDMA();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  
  // Setup touchscreen
  Wire.begin(SDA, SCL, 400000); // Configure i2c pins and set frequency to 400kHz
  
  // Setup LVGL
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, bufA, bufB, screenWidth * screenHeight / 10 );

  // Initialize the display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  // Initialize the touchscreen driver
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );

  // --- LVGL UI Configuration ---  
  
  // Set the background color
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);



  // Add content to the settings tab
  // With a flex layout, setting groups/boxes will position themselves automatically
  

  ChecksTable = lv_table_create(lv_scr_act());
  lv_table_set_col_width(ChecksTable, 0, 140);
  lv_table_set_col_width(ChecksTable, 1, 60);
  lv_table_set_col_cnt(ChecksTable, 2);
  lv_obj_remove_style(ChecksTable, NULL, LV_PART_ITEMS | LV_STATE_PRESSED);

  lv_table_set_cell_value_fmt(ChecksTable, 0, 0, "IR LED");
  lv_table_set_cell_value_fmt(ChecksTable, 1, 0, "IR Receiver");
  lv_table_set_cell_value_fmt(ChecksTable, 2, 0, "WiFi");
  lv_table_set_cell_value_fmt(ChecksTable, 3, 0, "Buttons");
  lv_table_set_cell_value_fmt(ChecksTable, 4, 0, "Touchscreen");
  lv_table_set_cell_value_fmt(ChecksTable, 5, 0, "Fuel Gauge");
  lv_table_set_cell_value_fmt(ChecksTable, 6, 0, "IMU");

  for(int i = 0; i < 7; i++){ //lv_table_get_row_cnt(table)
    lv_table_set_cell_value_fmt(ChecksTable, i, 1, "?");
  }

  










  // --- End of LVGL configuration ---


  #ifdef ENABLE_WIFI
  // Setup WiFi
  WiFi.setHostname("OMOTE"); //define hostname
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(true);
  #endif

  // Setup IMU
  IMU.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  IMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  IMU.settings.adcEnabled = 0;
  IMU.settings.tempEnabled = 0;
  IMU.settings.xAccelEnabled = 1;
  IMU.settings.yAccelEnabled = 1;
  IMU.settings.zAccelEnabled = 1;
  int IMUInitSuccessful = IMU.begin();  
  uint8_t intDataRead;
  IMU.readRegister(&intDataRead, LIS3DH_INT1_SRC);//clear interrupt
  
  // Setup IR
  IrSender.begin();
  digitalWrite(IR_VCC, HIGH); // Turn on IR receiver
  IrReceiver.enableIRIn();  // Start the receiver

  // Setup Fuel Gauge IC
  FuelGaugeInitSuccessful = fuelGauge.begin();


  lv_timer_handler(); // Run the LVGL UI once before the loop takes over


  Serial.print("Setup finished in ");
  Serial.print(millis());
  Serial.println("ms.");


  // Automated Checks
  uint64_t _chipmacid = 0LL;
  esp_efuse_mac_get_default((uint8_t*) (&_chipmacid));
  Serial.print("ESP32 MAC: ");
  Serial.println(_chipmacid);
  // Check if the touchscreen is responding
  boolean TouchInitSuccessful = false;
  Wire.beginTransmission(0x38);
  if(Wire.endTransmission() == 0) TouchInitSuccessful = true;

  
  if(IMUInitSuccessful == 0) lv_table_set_cell_value_fmt(ChecksTable, 6, 1, LV_SYMBOL_OK);
  else lv_table_set_cell_value_fmt(ChecksTable, 6, 1, LV_SYMBOL_WARNING);

  if(TouchInitSuccessful) lv_table_set_cell_value_fmt(ChecksTable, 4, 1, LV_SYMBOL_OK);
  else lv_table_set_cell_value_fmt(ChecksTable, 4, 1, LV_SYMBOL_WARNING);

  if(FuelGaugeInitSuccessful) lv_table_set_cell_value_fmt(ChecksTable, 5, 1, LV_SYMBOL_OK);
  else lv_table_set_cell_value_fmt(ChecksTable, 5, 1, LV_SYMBOL_WARNING);

  lv_table_set_cell_value_fmt(ChecksTable, 2, 1, LV_SYMBOL_WARNING);

}

// Loop ------------------------------------------------------------------------------------------------------------------------------------

void loop() { 

  // Update Backlight brightness
  static int fadeInTimer = millis(); // fadeInTimer = time after setup
  if(millis() < fadeInTimer + backlight_brightness){ // Fade in the backlight brightness
    ledcWrite(5, millis()-fadeInTimer);
  }
  else { // Dim Backlight before entering standby    
    if(standbyTimer < 2000) ledcWrite(5, 85); // Backlight dim
    else ledcWrite(5, backlight_brightness);  // Backlight on 
  }

  // Update LVGL UI
  lv_timer_handler(); 

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

  // Update battery stats at 1Hz
  static unsigned long batteryTaskTimer = millis() + 1000; // add 1s to start immediately
  if(FuelGaugeInitSuccessful && (millis() - batteryTaskTimer >= 1000)){
    battery_voltage = (int)(fuelGauge.getVoltage()*1000);
    battery_percentage = (int)fuelGauge.getSOC();
    battery_ischarging = !digitalRead(CRG_STAT);
    batteryTaskTimer = millis();
  }

  // Keypad Handling
  customKeypad.getKey(); // Populate key list
  for(int i=0; i<LIST_MAX; i++){ // Handle multiple keys (Not really necessary in this case)
    if(customKeypad.key[i].kstate == PRESSED || customKeypad.key[i].kstate == HOLD){
      standbyTimer = SLEEP_TIMEOUT; // Reset the sleep timer when a button is pressed
      int keyCode = customKeypad.key[i].kcode;
      //Serial.println(customKeypad.key[i].kchar);
      // Send IR codes depending on the current device (tabview page)
      if(currentDevice == 1) IrSender.sendRC5(IrSender.encodeRC5X(0x00, keyMapTechnisat[keyCode/ROWS][keyCode%ROWS]));
      else if(currentDevice == 2) IrSender.sendSony((keyCode/ROWS)*(keyCode%ROWS), 15);

      int missingKeys = 24;
      for(int k=0; k<5; k++){
        for(int l=0; l<5; l++){
          if(hexaKeys[k][l] == customKeypad.key[i].kchar) keysWorking[k][l] = 1;
          if(keysWorking[k][l] == 1) missingKeys--;
        }
      }
      
      if(missingKeys==0) lv_table_set_cell_value_fmt(ChecksTable, 3, 1, LV_SYMBOL_OK);
      else lv_table_set_cell_value_fmt(ChecksTable, 3, 1, "%d", missingKeys);
    }
  }

  // IR Test
  decode_results results;
  if (IrReceiver.decode(&results)) {
    //Serial.println(String(results.command));
    lv_table_set_cell_value_fmt(ChecksTable, 0, 1, LV_SYMBOL_OK);
    lv_table_set_cell_value_fmt(ChecksTable, 1, 1, LV_SYMBOL_OK);
    IrReceiver.resume(); // Enable receiving of the next value
  }

  
  


}