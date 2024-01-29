#include <lvgl.h>
#include "hardware/tft.h"
#include "hardware/sleep.h"
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/guiRegistry.h"

lv_obj_t* panel;
lv_color_t color_primary = lv_color_hex(0x303030); // gray
lv_obj_t* WifiLabel = NULL;
lv_obj_t* BluetoothLabel;
lv_obj_t* objBattPercentage;
lv_obj_t* objBattIcon;
lv_obj_t* SceneLabel;
byte currentScreen = 1; // Current Device to control (allows switching mappings between devices)

// LVGL declarations
LV_IMG_DECLARE(gradientLeft);
LV_IMG_DECLARE(gradientRight);

// Helper Functions -----------------------------------------------------------------------------------------------------------------------

// Set the page indicator scroll position relative to the tabview scroll position
static void store_scroll_value_event_cb(lv_event_t* e){
  float bias = (150.0 + 8.0) / 240.0;
  int offset = 240 / 2 - 150 / 2 - 8 - 50 - 3;
  lv_obj_t* screen = lv_event_get_target(e);
  lv_obj_scroll_to_x(panel, lv_obj_get_scroll_x(screen) * bias - offset, LV_ANIM_OFF);
}

// Update current screen when the tabview page is changes
static void tabview_device_event_cb(lv_event_t* e){
  currentScreen = lv_tabview_get_tab_act(lv_event_get_target(e));
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
    resetStandbyTimer();
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

static lv_disp_draw_buf_t draw_buf;
// static lv_color_t bufA[ screenWidth * screenHeight / 10 ];
// static lv_color_t bufB[ screenWidth * screenHeight / 10 ];

void init_gui(void) {

  // Setup LVGL ---------------------------------------------------------------------------------------------
  lv_init();

  lv_color_t * bufA = (lv_color_t *) malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 10);
  lv_color_t * bufB = (lv_color_t *) malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 10);

  lv_disp_draw_buf_init( &draw_buf, bufA, bufB, screenWidth * screenHeight / 10 );

  // Initialize the display driver --------------------------------------------------------------------------
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

  // Set the background color -------------------------------------------------------------------------------
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);

  // Setup a scrollable tabview for devices and settings ----------------------------------------------------
  lv_obj_t* tabview;
  tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0); // Hide tab labels by setting their height to 0
  lv_obj_set_style_bg_color(tabview, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_size(tabview, screenWidth, 270); // 270 = screenHeight(320) - panel(30) - statusbar(20)
  lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);

  // Add all the tabs here
  create_gui_tabs_from_gui_registry(tabview);

  // Set current page according to the current screen
  lv_tabview_set_act(tabview, currentScreen, LV_ANIM_OFF); 


  // Create a page indicator at the bottom ------------------------------------------------------------------
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
  create_gui_pageIndicators_from_gui_registry(panel);
  
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


  // Create a status bar at the top -------------------------------------------------------------------------
  lv_obj_t* statusbar = lv_btn_create(lv_scr_act());
  lv_obj_set_size(statusbar, 240, 20);
  lv_obj_set_style_shadow_width(statusbar, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(statusbar, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
  lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, 0);

  // WiFi -------------------------------------------------------------------------
  WifiLabel = lv_label_create(statusbar);
  lv_label_set_text(WifiLabel, "");
  lv_obj_align(WifiLabel, LV_ALIGN_LEFT_MID, -8, 0);
  lv_obj_set_style_text_font(WifiLabel, &lv_font_montserrat_12, LV_PART_MAIN);

  // Bluetooth --------------------------------------------------------------------
  BluetoothLabel = lv_label_create(statusbar);
  lv_label_set_text(BluetoothLabel, "");
  lv_obj_align(BluetoothLabel, LV_ALIGN_LEFT_MID, 12, 0);
  lv_obj_set_style_text_font(BluetoothLabel, &lv_font_montserrat_12, LV_PART_MAIN);

  // Scene ------------------------------------------------------------------------
  SceneLabel = lv_label_create(statusbar);
  lv_label_set_text(SceneLabel, "");
  lv_obj_align(SceneLabel, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(SceneLabel, &lv_font_montserrat_12, LV_PART_MAIN);

  // Battery ----------------------------------------------------------------------
  objBattPercentage = lv_label_create(statusbar);
  lv_label_set_text(objBattPercentage, "");
//  lv_obj_align(objBattPercentage, LV_ALIGN_RIGHT_MID, -16, 0);
  lv_obj_align(objBattPercentage, LV_ALIGN_RIGHT_MID, -20, 0);
  lv_obj_set_style_text_font(objBattPercentage, &lv_font_montserrat_12, LV_PART_MAIN);
  objBattIcon = lv_label_create(statusbar);
  lv_label_set_text(objBattIcon, LV_SYMBOL_BATTERY_EMPTY);
  lv_obj_align(objBattIcon, LV_ALIGN_RIGHT_MID, 8, 0);
  lv_obj_set_style_text_font(objBattIcon, &lv_font_montserrat_16, LV_PART_MAIN);

  // --- End of LVGL configuration ---

}

void gui_loop(void) {
  lv_timer_handler();
}
