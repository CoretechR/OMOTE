
#include <Arduino.h>
#include "display.hpp"
#include "Wire.h"
#include <Preferences.h>


extern bool wakeupByIMUEnabled;
extern Display display;
// LVGL declarations
LV_IMG_DECLARE(gradientLeft);
LV_IMG_DECLARE(gradientRight);
LV_IMG_DECLARE(appleTvIcon);
LV_IMG_DECLARE(appleDisplayIcon);
LV_IMG_DECLARE(appleBackIcon);
LV_IMG_DECLARE(lightbulb);
LV_IMG_DECLARE(WiFi_No_Signal);
extern lv_obj_t* panel;
static lv_disp_drv_t disp_drv;
/*TODO: get rid of global variable and use API functions instead*/
extern Preferences preferences;
//TODO: fix callback function structure to pass it in and/or move it out of display class somehow else
void smartHomeSlider_event_cb(lv_event_t * e);
void smartHomeToggle_event_cb(lv_event_t * e);
void WakeEnableSetting_event_cb(lv_event_t * e);
void appleKey_event_cb(lv_event_t* e);
void virtualKeypad_event_cb(lv_event_t* e);
void bl_slider_event_cb(lv_event_t * e);
void tabview_device_event_cb(lv_event_t* e);
void store_scroll_value_event_cb(lv_event_t* e);
void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

// Display flushing
static void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ){
  display.flush(disp, area, color_p);
}

Display::Display(int backlight_pin, int enable_pin, int width, int height)
{
  this->backlight_pin = backlight_pin;
  this->enable_pin = enable_pin;
  this->width = width;
  this->height = height;
  this->primary_color = lv_color_hex(0x303030); // gray

  /*Initialize the keybard as null*/
  this->kb = NULL;
}

void Display::hide_keyboard()
{
  lv_obj_add_flag(this->kb, LV_OBJ_FLAG_HIDDEN);
}

void Display::show_keyboard()
{
  lv_obj_clear_flag(this->kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(this->kb);
}

void Display::reset_settings_menu()
{
  lv_menu_set_page(this->settingsMenu, this->settingsMainPage);
}

void Display::turnOff()
{
  digitalWrite(this->backlight_pin, HIGH);
  digitalWrite(this->enable_pin, HIGH);
  pinMode(this->backlight_pin, INPUT);
  pinMode(this->enable_pin, INPUT);
  gpio_hold_en((gpio_num_t) this->backlight_pin);
  gpio_hold_en((gpio_num_t) this->enable_pin);
  preferences.putUChar("blBrightness", this->backlight_brightness);
}

void Display::setup()
{
  // LCD Pin Definition
  pinMode(this->enable_pin, OUTPUT);
  digitalWrite(this->enable_pin, HIGH);
  pinMode(this->backlight_pin, OUTPUT);
  digitalWrite(this->backlight_pin, HIGH);

  this->tft = TFT_eSPI();
 // Configure the backlight PWM
  // Manual setup because ledcSetup() briefly turns on the backlight
  ledc_channel_config_t ledc_channel_left;
  ledc_channel_left.gpio_num = (gpio_num_t)this->backlight_pin;
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

  // Slowly charge the VSW voltage to prevent a brownout
  // Workaround for hardware rev 1!
  for(int i = 0; i < 100; i++){
    digitalWrite(this->enable_pin, HIGH);  // LCD Logic off
    delayMicroseconds(1);
    digitalWrite(this->enable_pin, LOW);  // LCD Logic on
  }  

  delay(100); // Wait for the LCD driver to power on
  this->tft.init();
  this->tft.initDMA();
  this->tft.setRotation(0);
  this->tft.fillScreen(TFT_BLACK);
  this->tft.setSwapBytes(true);

  //TODO: move touchscreen handling out of Display class
  // Setup touchscreen
  this->touch = Adafruit_FT6206();
  Wire.begin(19, 22, 400000); // Configure i2c pins and set frequency to 400kHz
  this->touch.begin(128); // Initialize touchscreen and set sensitivity threshold

  this->backlight_brightness = preferences.getUChar("blBrightness", DEFAULT_BACKLIGHT_BRIGHTNESS);
  
  // Setup LVGL
  lv_init();

}

  static lv_disp_draw_buf_t draw_buf;
void Display::setup_ui(){
  this->bufA = (lv_color_t*) malloc((this->width*this->height / 10)* sizeof(lv_color_t));
  this->bufB = (lv_color_t*) malloc((this->width*this->height / 10)* sizeof(lv_color_t));

  lv_disp_draw_buf_init( &draw_buf, this->bufA, this->bufB, this->width * this->height / 10 );
  // Initialize the display driver
  lv_disp_drv_init( &disp_drv );
  disp_drv.hor_res = this->width;
  disp_drv.ver_res = this->height;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  //TODO: move touchscreen driver to its own module
  // Initialize the touchscreen driver
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );
  // Set the background color
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);

  this->create_keyboard();
  // Setup a scrollable tabview for devices and settings
  lv_obj_t* tabview;
  tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0); // Hide tab labels by setting their height to 0
  lv_obj_set_style_bg_color(tabview, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_size(tabview, this->width, 270); // 270 = screenHeight(320) - panel(30) - statusbar(20)
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
    lv_obj_set_style_bg_color(obj, this->primary_color, LV_PART_MAIN);
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
  lv_obj_set_style_bg_color(button, this->primary_color, LV_PART_MAIN);
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
  lv_obj_set_style_bg_color(button, this->primary_color, LV_PART_MAIN);
  lv_obj_add_event_cb(button, appleKey_event_cb, LV_EVENT_CLICKED, (void*)2);

  appleImg = lv_img_create(button);
  lv_img_set_src(appleImg, &appleDisplayIcon);
  lv_obj_set_style_img_recolor(appleImg, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(appleImg, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(appleImg, LV_ALIGN_CENTER, 0, 0);

  this->setup_settings(tab1);

  // Add content to the smart home tab (4)
  lv_obj_set_layout(tab4, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(tab4, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(tab4, LV_SCROLLBAR_MODE_ACTIVE);

  // Add a label, then a box for the light controls
  lv_obj_t* menuLabel = lv_label_create(tab4);
  lv_label_set_text(menuLabel, "Living Room");

  lv_obj_t* menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, this->primary_color, LV_PART_MAIN);
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
  lv_obj_set_style_bg_color(lightToggleA, lv_color_lighten(this->primary_color, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleA, this->primary_color, LV_PART_INDICATOR);
  lv_obj_add_event_cb(lightToggleA, smartHomeToggle_event_cb, LV_EVENT_VALUE_CHANGED, (void*)1);

  lv_obj_t *slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 0, 100);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(this->primary_color, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(slider, smartHomeSlider_event_cb, LV_EVENT_VALUE_CHANGED, (void*)1);

  // Add another this->settingsMenu box for a second appliance
  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, this->primary_color, LV_PART_MAIN);
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
  lv_obj_set_style_bg_color(lightToggleB, lv_color_lighten(this->primary_color, 50), LV_PART_MAIN);
  lv_obj_set_style_bg_color(lightToggleB, this->primary_color, LV_PART_INDICATOR);
  lv_obj_add_event_cb(lightToggleB, smartHomeToggle_event_cb, LV_EVENT_VALUE_CHANGED, (void*)2);

  slider = lv_slider_create(menuBox);
  lv_slider_set_range(slider, 0, 100);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(lv_color_black(), 30), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(slider, lv_color_lighten(lv_palette_main(LV_PALETTE_AMBER), 180), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);
  lv_obj_set_style_bg_color(slider, lv_color_white(), LV_PART_KNOB);
  lv_obj_set_style_bg_opa(slider, 255, LV_PART_MAIN);
  lv_obj_set_style_bg_color(slider, lv_color_lighten(this->primary_color, 50), LV_PART_MAIN);
  lv_slider_set_value(slider, 255, LV_ANIM_OFF);
  lv_obj_set_size(slider, lv_pct(90), 10);
  lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 37);
  lv_obj_add_event_cb(slider, smartHomeSlider_event_cb, LV_EVENT_VALUE_CHANGED, (void*)2);


  // Add another room (empty for now)
  menuLabel = lv_label_create(tab4);
  lv_label_set_text(menuLabel, "Kitchen");

  menuBox = lv_obj_create(tab4);
  lv_obj_set_size(menuBox, lv_pct(100), 79);
  lv_obj_set_style_bg_color(menuBox, this->primary_color, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);


  // Set current page according to the current Device
  lv_tabview_set_act(tabview, 0, LV_ANIM_OFF); 


  // Create a page indicator
  panel = lv_obj_create(lv_scr_act());
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_CLICKABLE); // This indicator will not be clickable
  lv_obj_set_size(panel, this->width, 30);
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
  lv_obj_set_style_bg_color(btn, this->primary_color, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Technisat");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, this->primary_color, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Apple TV");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, this->primary_color, LV_PART_MAIN);

  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  label = lv_label_create(btn);
  lv_label_set_text_fmt(label, "Smart Home");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, this->primary_color, LV_PART_MAIN);
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

  this->WifiLabel = lv_label_create(statusbar);
  lv_label_set_text(this->WifiLabel, "");
  lv_obj_align(this->WifiLabel, LV_ALIGN_LEFT_MID, -8, 0);
  lv_obj_set_style_text_font(this->WifiLabel, &lv_font_montserrat_12, LV_PART_MAIN);

  this->objBattPercentage = lv_label_create(statusbar);
  lv_label_set_text(this->objBattPercentage, "");
  lv_obj_align(this->objBattPercentage, LV_ALIGN_RIGHT_MID, -16, 0);
  lv_obj_set_style_text_font(this->objBattPercentage, &lv_font_montserrat_12, LV_PART_MAIN);

  this->objBattIcon = lv_label_create(statusbar);
  lv_label_set_text(this->objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
  lv_obj_align(this->objBattIcon, LV_ALIGN_RIGHT_MID, 8, 0);
  lv_obj_set_style_text_font(this->objBattIcon, &lv_font_montserrat_16, LV_PART_MAIN);

}


lv_obj_t* wifi_subpage;
static lv_obj_t * kb;
static lv_obj_t * ta;

/* Callback function to show and hide keybaord for attached textareas */
static void ta_kb_event_cb(lv_event_t* e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  lv_obj_t * kb = (lv_obj_t*) lv_event_get_user_data(e);
  switch(code){
    case LV_EVENT_FOCUSED:
      lv_keyboard_set_textarea(kb, ta);
      lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(kb);
      break;
    case LV_EVENT_DEFOCUSED:
      lv_keyboard_set_textarea(kb, NULL);
      lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
      break;
    default:
      break;
  }
}

void Display::create_keyboard()
{
  this->kb = lv_keyboard_create(lv_scr_act());
  lv_obj_add_flag(this->kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_y(this->kb, 0);
}

void Display::attach_keyboard(lv_obj_t* textarea)
{
  if (this->kb == NULL)
  {
    this->create_keyboard();
  }
  lv_keyboard_set_textarea(this->kb, textarea);
  lv_obj_add_event_cb(textarea, ta_kb_event_cb, LV_EVENT_FOCUSED, this->kb);
  lv_obj_add_event_cb(textarea, ta_kb_event_cb, LV_EVENT_DEFOCUSED, this->kb);
}

void Display::setup_settings(lv_obj_t* parent)
{
  // Add content to the settings tab
  // With a flex layout, setting groups/boxes will position themselves automatically
  lv_obj_set_layout(parent, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_ACTIVE);
  // Add a label, then a box for the display settings
  this->settingsMenu = lv_menu_create(parent);
  lv_obj_set_width(this->settingsMenu, 210);

  /* Create main page for settings this->settingsMenu*/
  this->settingsMainPage = lv_menu_page_create(this->settingsMenu, NULL);
  lv_obj_t* cont = lv_menu_cont_create(this->settingsMainPage);
  lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_ACTIVE);
  //lv_obj_set_width(cont, lv_obj_get_width(parent));
  this->display_settings(cont);

  this->create_wifi_settings(this->settingsMenu, cont);

  // Another setting for the battery
  lv_obj_t* menuLabel = lv_label_create(cont);
  lv_label_set_text(menuLabel, "Battery");
  lv_obj_t* menuBox = lv_obj_create(cont);
  lv_obj_set_size(menuBox, lv_pct(100), 125);
  lv_obj_set_style_bg_color(menuBox, this->primary_color, LV_PART_MAIN);
  lv_obj_set_style_border_width(menuBox, 0, LV_PART_MAIN);

  lv_menu_set_page(this->settingsMenu, this->settingsMainPage);
}

void Display::flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ){
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  this->tft.startWrite();
  this->tft.setAddrWindow( area->x1, area->y1, w, h );
  this->tft.pushPixelsDMA( ( uint16_t * )&color_p->full, w * h);
  this->tft.endWrite();

  lv_disp_flush_ready( disp );
}

void Display::update()
{
  
  static int fadeInTimer = millis(); // fadeInTimer = time after setup
  if(millis() < fadeInTimer + backlight_brightness){ // Fade in the backlight brightness
    ledcWrite(5, millis()-fadeInTimer);
  }
  else { // Dim Backlight before entering standby    
    //if(standbyTimer < 2000) ledcWrite(5, 85); // Backlight dim
    //else ledcWrite(5, this->backlight_brightness);  // Backlight on 
    ledcWrite(5, this->backlight_brightness);  // Backlight on 
  }
  // Update LVGL UI
  lv_timer_handler(); 
}

void Display::update_battery(int percentage, bool isCharging, bool isConnected)
{
  if(isCharging || !isConnected)
  {
    Serial.println("charging?");
    lv_label_set_text(this->objBattPercentage, "");
    lv_label_set_text(this->objBattIcon, LV_SYMBOL_USB);
  }
  else {
    Serial.println("Running on battery");
    if(percentage > 95){
      lv_label_set_text(this->objBattIcon, LV_SYMBOL_BATTERY_FULL);
    } else if(percentage > 75)
    {
      lv_label_set_text(this->objBattIcon, LV_SYMBOL_BATTERY_3);
    } else if(percentage > 50) 
    {
      lv_label_set_text(this->objBattIcon, LV_SYMBOL_BATTERY_2);
    } else if(percentage > 25) 
    {
      lv_label_set_text(this->objBattIcon, LV_SYMBOL_BATTERY_1);
    } else {
      lv_label_set_text(this->objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
    }
  }
}
