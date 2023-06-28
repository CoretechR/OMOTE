// OMOTE firmware for ESP32
// 2023 Maximilian Kern

#include <TFT_eSPI.h> // Hardware-specific library
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

// LCD declarations
TFT_eSPI tft = TFT_eSPI();
#define screenWidth 240
#define screenHeight 320
Adafruit_FT6206 touch = Adafruit_FT6206();
TS_Point touchPoint;
TS_Point oldPoint;
int backlight_brightness = 255;

// LVGL declarations
static lv_disp_draw_buf_t draw_buf;
static lv_color_t bufA[ screenWidth * screenHeight / 10 ];
static lv_color_t bufB[ screenWidth * screenHeight / 10 ];
lv_obj_t* objBattPercentage;
lv_obj_t* objBattIcon;
LV_IMG_DECLARE(gradientLeft);
LV_IMG_DECLARE(gradientRight);
LV_IMG_DECLARE(appleTvIcon);
LV_IMG_DECLARE(appleDisplayIcon);
LV_IMG_DECLARE(appleBackIcon);
LV_IMG_DECLARE(high_brightness);
LV_IMG_DECLARE(low_brightness);
LV_IMG_DECLARE(lightbulb);
lv_obj_t* panel;
lv_color_t color_primary = lv_color_hex(0x303030); // gray

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

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define MQTT_SERVER "YOUR_MQTT_SERVER_IP"
lv_obj_t* WifiLabel;
WiFiClient espClient;
PubSubClient client(espClient);

// Helper Functions -----------------------------------------------------------------------------------------------------------------------

// Set the page indicator scroll position relative to the tabview scroll position
static void store_scroll_value_event_cb(lv_event_t* e){
  float bias = (150.0 + 8.0) / 240.0;
  int offset = 240 / 2 - 150 / 2 - 8 - 50 - 3;
  lv_obj_t* screen = lv_event_get_target(e);
  lv_obj_scroll_to_x(panel, lv_obj_get_scroll_x(screen) * bias - offset, LV_ANIM_OFF);
}

// Update current device when the tabview page is changes
static void tabview_device_event_cb(lv_event_t* e){
  currentDevice = lv_tabview_get_tab_act(lv_event_get_target(e));
}

// Slider Event handler
static void bl_slider_event_cb(lv_event_t * e){
  lv_obj_t * slider = lv_event_get_target(e);
  backlight_brightness = constrain(lv_slider_get_value(slider), 60, 255);
}

// Virtual Keypad Event handler
static void virtualKeypad_event_cb(lv_event_t* e) {
  lv_obj_t* target = lv_event_get_target(e);
  lv_obj_t* cont = lv_event_get_current_target(e);
  if (target == cont) return; // stop if container was clicked
  Serial.println(virtualKeyMapTechnisat[(int)target->user_data]);
  // Send IR command based on the button user data  
  IrSender.sendRC5(IrSender.encodeRC5X(0x00, virtualKeyMapTechnisat[(int)target->user_data]));
}

// Apple Key Event handler
static void appleKey_event_cb(lv_event_t* e) {
  // Send IR command based on the event user data  
  IrSender.sendSony(50 + (int)e->user_data, 15);
  Serial.println(50 + (int)e->user_data);
}

// Wakeup by IMU Switch Event handler
static void WakeEnableSetting_event_cb(lv_event_t * e){
  wakeupByIMUEnabled = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
}

// Smart Home Toggle Event handler
static void smartHomeToggle_event_cb(lv_event_t * e){
  char payload[8];
  if(lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED)) strcpy(payload,"true");
  else strcpy(payload,"false");
  // Publish an MQTT message based on the event user data  
  if((int)e->user_data == 1) client.publish("bulb1_set", payload);
  if((int)e->user_data == 2) client.publish("bulb2_set", payload);
}

// Smart Home Toggle Event handler
static void smartHomeSlider_event_cb(lv_event_t * e){
  lv_obj_t * slider = lv_event_get_target(e);
  char payload[8];
  dtostrf(lv_slider_get_value(slider), 1, 2, payload);
  // Publish an MQTT message based on the event user data
  if((int)e->user_data == 1) client.publish("bulb1_setbrightness", payload);
  if((int)e->user_data == 2) client.publish("bulb2_setbrightness", payload);
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
    client.setServer(MQTT_SERVER, 1883); // MQTT initialization
    client.connect("OMOTE"); // Connect using a client id
  }
  // Set status bar icon based on WiFi status
  if(event == ARDUINO_EVENT_WIFI_STA_GOT_IP || event == ARDUINO_EVENT_WIFI_STA_GOT_IP6){
    lv_label_set_text(WifiLabel, LV_SYMBOL_WIFI);
  }
  else{
    lv_label_set_text(WifiLabel, "");
  }
}
#endif

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
  if(preferences.getBool("alreadySetUp")){
    wakeupByIMUEnabled = preferences.getBool("wkpByIMU");
    backlight_brightness = preferences.getUChar("blBrightness");
    currentDevice = preferences.getUChar("currentDevice");
  }  

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
  touch.begin(128); // Initialize touchscreen and set sensitivity threshold
  
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

  // Setup a scrollable tabview for devices and settings
  lv_obj_t* tabview;
  tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0); // Hide tab labels by setting their height to 0
  lv_obj_set_style_bg_color(tabview, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_size(tabview, screenWidth, 270); // 270 = screenHeight(320) - panel(30) - statusbar(20)
  lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);

  // Add 4 tabs (names are irrelevant since the labels are hidden)
  lv_obj_t* tab1 = lv_tabview_add_tab(tabview, "Settings");
  lv_obj_t* tab2 = lv_tabview_add_tab(tabview, "Technisat");
  lv_obj_t* tab3 = lv_tabview_add_tab(tabview, "Apple TV");
  lv_obj_t* tab4 = lv_tabview_add_tab(tabview, "Smart Home");

  // Configure number button grid 
  static lv_coord_t col_dsc[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST }; // equal x distribution
  static lv_coord_t row_dsc[] = { 52, 52, 52, 52, LV_GRID_TEMPLATE_LAST }; // manual y distribution to compress the grid a bit

  // Create a container with grid for tab2
  lv_obj_set_style_pad_all(tab2, 0, LV_PART_MAIN);
  lv_obj_t* cont = lv_obj_create(tab2);
  lv_obj_set_style_shadow_width(cont, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(cont, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
  lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
  lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
  lv_obj_set_size(cont, 240, 270);
  lv_obj_set_layout(cont, LV_LAYOUT_GRID);
  lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_radius(cont, 0, LV_PART_MAIN);

  lv_obj_t* buttonLabel;
  lv_obj_t* obj;

  // Iterate through grid buttons configure them
  for (int i = 0; i < 12; i++) {
    uint8_t col = i % 3;
    uint8_t row = i / 3;
    // Create the button object
    if ((row == 3) && ((col == 0) || (col == 2))) continue; // Do not create a complete fourth row, only a 0 button
    obj = lv_btn_create(cont);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
    lv_obj_set_style_bg_color(obj, color_primary, LV_PART_MAIN);
    lv_obj_set_style_radius(obj, 14, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(obj, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE); // Clicking a button causes a event in its container
    // Create Labels for each button
    buttonLabel = lv_label_create(obj);        
    if(i < 9){
      lv_label_set_text_fmt(buttonLabel, std::to_string(i+1).c_str(), col, row);
      lv_obj_set_user_data(obj, (void*)i); // Add user data so we can identify which button caused the container event
    }
    else{
      lv_label_set_text_fmt(buttonLabel, "0", col, row);
      lv_obj_set_user_data(obj, (void*)9);
    } 
    lv_obj_set_style_text_font(buttonLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_center(buttonLabel);
  }
  // Create a shared event for all button inside container
  lv_obj_add_event_cb(cont, virtualKeypad_event_cb, LV_EVENT_CLICKED, NULL);
  

  // Add content to the Apple TV tab (3)
  // Add a nice apple tv logo
  lv_obj_t* appleImg = lv_img_create(tab3);
  lv_img_set_src(appleImg, &appleTvIcon);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, -60);
  // create two buttons and add their icons accordingly
  lv_obj_t* button = lv_btn_create(tab3);
  lv_obj_align(button, LV_ALIGN_BOTTOM_LEFT, 10, 0);
  lv_obj_set_size(button, 60, 60);
  lv_obj_set_style_radius(button, 30, LV_PART_MAIN);
  lv_obj_set_style_bg_color(button, color_primary, LV_PART_MAIN);
  lv_obj_add_event_cb(button, appleKey_event_cb, LV_EVENT_CLICKED, (void*)1);

  appleImg = lv_img_create(button);
  lv_img_set_src(appleImg, &appleBackIcon);
  lv_obj_set_style_img_recolor(appleImg, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(appleImg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, -3, 0);

  button = lv_btn_create(tab3);
  lv_obj_align(button, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
  lv_obj_set_size(button, 60, 60);
  lv_obj_set_style_radius(button, 30, LV_PART_MAIN);
  lv_obj_set_style_bg_color(button, color_primary, LV_PART_MAIN);
  lv_obj_add_event_cb(button, appleKey_event_cb, LV_EVENT_CLICKED, (void*)2);

  appleImg = lv_img_create(button);
  lv_img_set_src(appleImg, &appleDisplayIcon);
  lv_obj_set_style_img_recolor(appleImg, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(appleImg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, 0);
  



  // Add content to the settings tab
  // With a flex layout, setting groups/boxes will position themselves automatically
  lv_obj_set_layout(tab1, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab1, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_ACTIVE);

  // Add a label, then a box for the display settings
  lv_obj_t* menuLabel = lv_label_create(tab1);
  lv_label_set_text(menuLabel, "Display");

  lv_obj_t* menuBox = lv_obj_create(tab1);
  lv_obj_set_size(menuBox, lv_pct(100), 109);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  lv_obj_t* brightnessIcon = lv_img_create(menuBox);
  lv_img_set_src(brightnessIcon, &low_brightness);
  lv_obj_set_style_img_recolor(brightnessIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(brightnessIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(brightnessIcon, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_t* slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 60, 255);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, backlight_brightness, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(66), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 3);
  brightnessIcon = lv_img_create(menuBox);
  lv_img_set_src(brightnessIcon, &high_brightness);
  lv_obj_set_style_img_recolor(brightnessIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(brightnessIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(brightnessIcon, LV_ALIGN_TOP_RIGHT, 0, -1);
  lv_obj_add_event_cb(slider, bl_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Lift to Wake");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 32);
  lv_obj_t* wakeToggle = lv_switch_create(menuBox);
  lv_obj_set_size(wakeToggle, 40, 22);
  lv_obj_align(wakeToggle, LV_ALIGN_TOP_RIGHT, 0, 29);
  lv_obj_set_style_bg_color(wakeToggle, lv_color_hex(0x505050), LV_PART_MAIN);
  lv_obj_add_event_cb(wakeToggle, WakeEnableSetting_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  if(wakeupByIMUEnabled) lv_obj_add_state(wakeToggle, LV_STATE_CHECKED); // set default state

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Timeout");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 64);
  lv_obj_t* drop = lv_dropdown_create(menuBox);
  lv_dropdown_set_options(drop, "10s\n"
                                "30s\n"
                                "1m\n"
                                "3m");
  lv_obj_align(drop, LV_ALIGN_TOP_RIGHT, 0, 61);
  lv_obj_set_size(drop, 70, 22);
  //lv_obj_set_style_text_font(drop, &lv_font_montserrat_12, LV_PART_MAIN);
  //lv_obj_set_style_text_font(lv_dropdown_get_list(drop), &lv_font_montserrat_12, LV_PART_MAIN);
  lv_obj_set_style_pad_top(drop, 1, LV_PART_MAIN);
  lv_obj_set_style_bg_color(drop, color_primary, LV_PART_MAIN);
  lv_obj_set_style_bg_color(lv_dropdown_get_list(drop), color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(lv_dropdown_get_list(drop), 1, LV_PART_MAIN);
  lv_obj_set_style_border_color(lv_dropdown_get_list(drop), lv_color_hex(0x505050), LV_PART_MAIN);

  // Add another label, then a settings box for WiFi
  menuLabel = lv_label_create(tab1);
  lv_label_set_text(menuLabel, "Wi-Fi");
  menuBox = lv_obj_create(tab1);
  lv_obj_set_size(menuBox, lv_pct(100), 80);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Network");
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, LV_SYMBOL_RIGHT);
  lv_obj_align(menuLabel, LV_ALIGN_TOP_RIGHT, 0, 0);
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Password");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 0, 32);
  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, LV_SYMBOL_RIGHT);
  lv_obj_align(menuLabel, LV_ALIGN_TOP_RIGHT, 0, 32);

  // Another setting for the battery
  menuLabel = lv_label_create(tab1);
  lv_label_set_text(menuLabel, "Battery");
  menuBox = lv_obj_create(tab1);
  lv_obj_set_size(menuBox, lv_pct(100), 125);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  



  // Add content to the smart home tab (4)
  lv_obj_set_layout(tab4, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab4, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab4, LV_SCROLLBAR_MODE_ACTIVE);

  // Add a label, then a box for the light controls
  menuLabel = lv_label_create(tab4);
  lv_label_set_text(menuLabel, "Living Room");

  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  lv_obj_t* bulbIcon = lv_img_create(menuBox);
  lv_img_set_src(bulbIcon, &lightbulb);
  lv_obj_set_style_img_recolor(bulbIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(bulbIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(bulbIcon, LV_ALIGN_TOP_LEFT, 0, 0);

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Floor Lamp");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 22, 3);
  lv_obj_t* lightToggleA = lv_switch_create(menuBox);
  lv_obj_set_size(lightToggleA, 40, 22);
  lv_obj_align(lightToggleA, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(lightToggleA, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleA, color_primary, LV_PART_INDICATOR);
  lv_obj_add_event_cb(lightToggleA, smartHomeToggle_event_cb, LV_EVENT_VALUE_CHANGED, (void*)1);

  slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 0, 100);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(slider, smartHomeSlider_event_cb, LV_EVENT_VALUE_CHANGED, (void*)1);

  // Add another menu box for a second appliance
  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  bulbIcon = lv_img_create(menuBox);
  lv_img_set_src(bulbIcon, &lightbulb);
  lv_obj_set_style_img_recolor(bulbIcon, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(bulbIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(bulbIcon, LV_ALIGN_TOP_LEFT, 0, 0);

  menuLabel = lv_label_create(menuBox);
  lv_label_set_text(menuLabel, "Ceiling Light");
  lv_obj_align(menuLabel, LV_ALIGN_TOP_LEFT, 22, 3);
  lv_obj_t* lightToggleB = lv_switch_create(menuBox);
  lv_obj_set_size(lightToggleB, 40, 22);
  lv_obj_align(lightToggleB, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_bg_color(lightToggleB, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleB, color_primary, LV_PART_INDICATOR);
  lv_obj_add_event_cb(lightToggleB, smartHomeToggle_event_cb, LV_EVENT_VALUE_CHANGED, (void*)2);

  slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 0, 100);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(color_primary, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(slider, smartHomeSlider_event_cb, LV_EVENT_VALUE_CHANGED, (void*)2);


  // Add another room (empty for now)
  menuLabel = lv_label_create(tab4);
  lv_label_set_text(menuLabel, "Kitchen");

  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, color_primary, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);


  // Set current page according to the current Device
  lv_tabview_set_act(tabview, currentDevice, LV_ANIM_OFF); 


  // Create a page indicator
  panel = lv_obj_create(lv_scr_act());
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_CLICKABLE); // This indicator will not be clickable
  lv_obj_set_size(panel, screenWidth, 30);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
  lv_obj_align(panel, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
  // This small hidden button enables the page indicator to scroll further
  lv_obj_t* btn = lv_btn_create(panel);
  lv_obj_set_size(btn, 50, lv_pct(100));
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
  // Create actual (non-clickable) buttons for every tab
  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  lv_obj_t* label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Settings");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Technisat");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Apple TV");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Smart Home");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);
  // This small hidden button enables the page indicator to scroll further
  btn = lv_btn_create(panel);
  lv_obj_set_size(btn, 50, lv_pct(100));
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
  
  // Make the indicator scroll together with the tabs by creating a scroll event
  lv_obj_add_event_cb(lv_tabview_get_content(tabview), store_scroll_value_event_cb, LV_EVENT_SCROLL, NULL);
  lv_obj_add_event_cb(tabview, tabview_device_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  // Initialize scroll position for the indicator
  lv_event_send(lv_tabview_get_content(tabview), LV_EVENT_SCROLL, NULL);

  // Style the panel background
  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_pad_all(&style_btn, 3);
  lv_style_set_border_width(&style_btn, 0);
  lv_style_set_bg_opa(&style_btn, LV_OPA_TRANSP);
  lv_obj_add_style(panel, &style_btn, 0);

  // Make the indicator fade out at the sides using gradient bitmaps
  lv_obj_t* img1 = lv_img_create(lv_scr_act());
  lv_img_set_src(img1, &gradientLeft);
  lv_obj_align(img1, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_set_size(img1, 30, 30); // stretch the 1-pixel high image to 30px
  lv_obj_t* img2 = lv_img_create(lv_scr_act());
  lv_img_set_src(img2, &gradientRight);
  lv_obj_align(img2, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_size(img2, 30, 30);


  // Create a status bar
  lv_obj_t* statusbar = lv_btn_create(lv_scr_act());
  lv_obj_set_size(statusbar, 240, 20);
  lv_obj_set_style_shadow_width(statusbar, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(statusbar, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
  lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, 0);

  WifiLabel = lv_label_create(statusbar);
  lv_label_set_text(WifiLabel, "");
  lv_obj_align(WifiLabel, LV_ALIGN_LEFT_MID, -8, 0);
  lv_obj_set_style_text_font(WifiLabel, &lv_font_montserrat_12, LV_PART_MAIN);



  

  objBattPercentage = lv_label_create(statusbar);
  lv_label_set_text(objBattPercentage, "");
  lv_obj_align(objBattPercentage, LV_ALIGN_RIGHT_MID, -16, 0);
  lv_obj_set_style_text_font(objBattPercentage, &lv_font_montserrat_12, LV_PART_MAIN);

  objBattIcon = lv_label_create(statusbar);
  lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
  lv_obj_align(objBattIcon, LV_ALIGN_RIGHT_MID, 8, 0);
  lv_obj_set_style_text_font(objBattIcon, &lv_font_montserrat_16, LV_PART_MAIN);

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
  if(millis() - batteryTaskTimer >= 1000){
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
  }

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