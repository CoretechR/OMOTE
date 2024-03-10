#include <lvgl.h>
#include "hardware/tft.h"
#include "hardware/sleep.h"
#include "hardware/memoryUsage.h"
#include "gui_general_and_keys/guiBase.h"
#include "gui_general_and_keys/guiRegistry.h"
#include "gui_general_and_keys/guiMemoryOptimizer.h"

lv_color_t color_primary = lv_color_hex(0x303030); // gray
lv_obj_t* MemoryUsageLabel = NULL;
lv_obj_t* WifiLabel = NULL;
lv_obj_t* BluetoothLabel = NULL;
lv_obj_t* BattPercentageLabel = NULL;
lv_obj_t* BattIconLabel = NULL;
lv_obj_t* SceneLabel = NULL;
uint32_t currentTabID = -1; // id of the current tab
uint32_t oldTabID = -1;

lv_obj_t* tabview = NULL;
// page indicator
lv_obj_t* panel = NULL;
lv_obj_t* img1 = NULL;
lv_obj_t* img2 = NULL;

static bool global_styles_already_initialized = false;
lv_style_t panel_style;
#ifdef drawRedBorderAroundMainWidgets
lv_style_t style_red_border;
#endif

// Helper Functions -----------------------------------------------------------------------------------------------------------------------

// Set the page indicator (panel) scroll position relative to the tabview content scroll position
// this is a callback if the CONTENT of the tabview is scrolled (LV_EVENT_SCROLL)
void tabview_content_is_scrolling_event_cb(lv_event_t* e){
  if (panel == NULL) { return;}

  float bias = float(150.0 + 8.0) / 240.0;
  //           screenwidth  indicator    ??    2 small hidden buttons  ??
  int offset = 240 / 2      - 150 / 2    - 8 - 2*50 / 2                -4;
  // get the object to which the event is sent
  lv_obj_t* tabviewContent = lv_event_get_target(e);
  
  // Get the x coordinate of object and scroll panel accordingly
  int16_t tabviewX = lv_obj_get_scroll_x(tabviewContent);
  // the last events we receive are 0, 238 and 476. But should be 0, 240 and 480. Otherwise page indicator jumps a litte bit after recreation of tabs.
  if ((tabviewX >= 237) && (tabviewX <= 240)) {tabviewX = 240;}
  if ((tabviewX >= 475) && (tabviewX <= 480)) {tabviewX = 480;}
  // we need 158 more (the size of one page indicator), because we always have one more page indicator at the beginning and at the end (so normally 5 when having 3 tabs)
  int16_t panelX = tabviewX * bias - offset + 158;
  // Serial.printf("scroll %d to %d\r\n", tabviewX, panelX);
  lv_obj_scroll_to_x(panel, panelX, LV_ANIM_OFF);
  // lv_obj_scroll_to_x(panel, lv_obj_get_scroll_x(tabviewContent) * bias - offset, LV_ANIM_OFF);
}

// -----------------------
static bool waitBeforeActionAfterSlidingAnimationEnded = false;
static unsigned long waitBeforeActionAfterSlidingAnimationEnded_timerStart;
// This is the callback when the animation of the tab sliding ended
void tabview_animation_ready_cb(lv_anim_t* a) {
  // Unfortunately, the animation has not completely ended here. We cannot do the recreation of the tabs here.
  // We have to wait some more milliseconds or at least one cycle of lv_timer_handler();
  // calling lv_timer_handler(); here does not help
  // lv_timer_handler();
  // guis_doAfterSliding(oldTabID, currentTabID);

  waitBeforeActionAfterSlidingAnimationEnded = true;
  waitBeforeActionAfterSlidingAnimationEnded_timerStart = millis();
}

// Update currentTabID when a new tab is selected
// this is a callback if the tabview is changed (LV_EVENT_VALUE_CHANGED)
// Sent when a new tab is selected by sliding or clicking the tab button. lv_tabview_get_tab_act(tabview) returns the zero based index of the current tab.
void tabview_tab_changed_event_cb(lv_event_t* e){
  if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
    
    oldTabID = currentTabID;
    currentTabID = lv_tabview_get_tab_act(lv_event_get_target(e));

    // Wait until the animation ended, then call "guis_doAfterSliding(oldTabID, currentTabID);"
    // https://forum.lvgl.io/t/delete-a-tab-after-the-tabview-scroll-animation-is-complete/3155/4
    lv_obj_t* myTabview = lv_event_get_target(e);
    lv_obj_t* tabContainer = lv_tabview_get_content(myTabview);
    // https://docs.lvgl.io/8.3/overview/animation.html?highlight=lv_anim_get#_CPPv411lv_anim_getPv18lv_anim_exec_xcb_t
    // (lv_anim_exec_xcb_t) lv_obj_set_x does not find an animation. NULL is good as well.
    lv_anim_t* anim = lv_anim_get(tabContainer, NULL); // (lv_anim_exec_xcb_t) lv_obj_set_x);
    if(anim) {
      lv_anim_set_ready_cb(anim, tabview_animation_ready_cb);
    }
  }
}

// Display flushing
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ){
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  #ifdef useTwoBuffersForlvgl
  tft.pushPixelsDMA((uint16_t*)&color_p->full, w * h);
  #else
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  #endif
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

void setMainWidgetsHeightAndPosition();
void init_gui_status_bar();
void init_gui_memoryUsage_bar();
void init_gui(void) {

  // Setup LVGL ---------------------------------------------------------------------------------------------
  lv_init();

  #ifdef useTwoBuffersForlvgl
  lv_color_t * bufA = (lv_color_t *) malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 10);
  lv_color_t * bufB = (lv_color_t *) malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 10);
  lv_disp_draw_buf_init(&draw_buf, bufA, bufB, screenWidth * screenHeight / 10);
  #else
  lv_color_t * bufA = (lv_color_t *) malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 10);
  lv_disp_draw_buf_init(&draw_buf, bufA, NULL, screenWidth * screenHeight / 10);
  #endif

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

  // -- draw red border around the main widgets -------------------------------------------------------------
  if (!global_styles_already_initialized) {
    // Style the panel background. Will be initialized only once and reused in fillPanelWithPageIndicator_strategyMax3
    lv_style_init(&panel_style);
    lv_style_set_pad_all(&panel_style, 3);
    lv_style_set_border_width(&panel_style, 0);
    lv_style_set_bg_opa(&panel_style, LV_OPA_TRANSP);
    #ifdef drawRedBorderAroundMainWidgets
    lv_style_init(&style_red_border);
    lv_style_set_border_side(&style_red_border, LV_BORDER_SIDE_FULL);
    lv_style_set_border_color(&style_red_border, lv_color_hex(0xff0000));
    lv_style_set_border_width(&style_red_border, 1);
    global_styles_already_initialized = true;
    #endif
  }
  // Set the background color -------------------------------------------------------------------------------
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
  // set default height and position of main widgets
  setMainWidgetsHeightAndPosition();
  // At startup, set current GUI according to currentGUIname, and create the content of that tab (and the previous and the next) for the first time
  guis_doTabCreationAtStartup();
  // memoryUsage bar
  init_gui_memoryUsage_bar();
  // status bar
  init_gui_status_bar();
}

int panelHeight;
int memoryUsageBarTop;
int memoryUsageBarHeight;
int statusbarTop;
int statusbarHeight;
int tabviewTop;
int tabviewHeight;
int labelsPositionTop;

void setMainWidgetsHeightAndPosition() {
  panelHeight          = 30;
  memoryUsageBarTop    = 0;
  memoryUsageBarHeight = getShowMemoryUsage() ? 14 : 0;
  statusbarTop         = memoryUsageBarTop + memoryUsageBarHeight;
  statusbarHeight      = 20;
  tabviewTop           = statusbarTop + statusbarHeight;
  tabviewHeight        = 320 - memoryUsageBarHeight - statusbarHeight - panelHeight;
  labelsPositionTop    = -2;
}

lv_obj_t* memoryUsageBar;
lv_obj_t* statusbar;

void showMemoryUsageBar(bool showBar) {
  setMainWidgetsHeightAndPosition();

  lv_obj_set_size(memoryUsageBar, 240, memoryUsageBarHeight);
  lv_obj_align(memoryUsageBar, LV_ALIGN_TOP_MID, 0, memoryUsageBarTop);
  lv_obj_set_size(statusbar, 240, statusbarHeight);
  lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, statusbarTop);
  lv_obj_set_size(tabview, screenWidth, tabviewHeight);
  lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, tabviewTop);

  return;

  if (showBar) {
    // lv_obj_clear_flag(memoryUsageBar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(memoryUsageBar, 240, memoryUsageBarHeight);
    lv_obj_align(memoryUsageBar, LV_ALIGN_TOP_MID, 0, memoryUsageBarTop);
    lv_obj_set_size(statusbar, 240, statusbarHeight);
    lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, statusbarTop);
    lv_obj_set_size(tabview, screenWidth, tabviewHeight);
    lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, tabviewTop);
  } else {
    // lv_obj_add_flag(memoryUsageBar, LV_OBJ_FLAG_HIDDEN);

  }
}

void init_gui_memoryUsage_bar() {
  // Create a memory status text bar at the top -------------------------------------------------------------
  memoryUsageBar = lv_btn_create(lv_scr_act());
  lv_obj_set_size(memoryUsageBar, 240, memoryUsageBarHeight);
  lv_obj_set_style_shadow_width(memoryUsageBar, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(memoryUsageBar, lv_color_black(), LV_PART_MAIN);
  #ifdef drawRedBorderAroundMainWidgets
  lv_obj_add_style(memoryUsageBar, &style_red_border, LV_PART_MAIN);
  #endif
  lv_obj_set_style_radius(memoryUsageBar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(memoryUsageBar, 0, LV_PART_MAIN);
  lv_obj_align(memoryUsageBar, LV_ALIGN_TOP_MID, 0, memoryUsageBarTop);

  MemoryUsageLabel = lv_label_create(memoryUsageBar);
  lv_label_set_text(MemoryUsageLabel, "");
  lv_obj_align(MemoryUsageLabel, LV_ALIGN_TOP_LEFT, 0, labelsPositionTop);
  lv_obj_set_style_text_font(MemoryUsageLabel, &lv_font_montserrat_12, LV_PART_MAIN);
  lv_label_set_recolor(MemoryUsageLabel, true);
}

void init_gui_status_bar() {
  // Create a status bar at the top -------------------------------------------------------------------------
  statusbar = lv_btn_create(lv_scr_act());
  lv_obj_set_size(statusbar, 240, statusbarHeight);
  lv_obj_set_style_shadow_width(statusbar, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(statusbar, lv_color_black(), LV_PART_MAIN);
  #ifdef drawRedBorderAroundMainWidgets
  lv_obj_add_style(statusbar, &style_red_border, LV_PART_MAIN);
  #endif
  lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(statusbar, 0, LV_PART_MAIN);
  lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, statusbarTop);

  int labelsPositionTopStatusbar = labelsPositionTop + 3;
  // WiFi -------------------------------------------------------------------------
  WifiLabel = lv_label_create(statusbar);
  lv_label_set_text(WifiLabel, "");
  lv_obj_align(WifiLabel, LV_ALIGN_TOP_LEFT, 0, labelsPositionTopStatusbar +1);
  lv_obj_set_style_text_font(WifiLabel, &lv_font_montserrat_12, LV_PART_MAIN);
  // Bluetooth --------------------------------------------------------------------
  BluetoothLabel = lv_label_create(statusbar);
  lv_label_set_text(BluetoothLabel, "");
  lv_obj_align(BluetoothLabel, LV_ALIGN_TOP_LEFT, 20, labelsPositionTopStatusbar);
  lv_obj_set_style_text_font(BluetoothLabel, &lv_font_montserrat_12, LV_PART_MAIN);
  // Scene ------------------------------------------------------------------------
  SceneLabel = lv_label_create(statusbar);
  lv_label_set_text(SceneLabel, "");
  lv_obj_align(SceneLabel, LV_ALIGN_TOP_MID, 0, labelsPositionTopStatusbar);
  lv_obj_set_style_text_font(SceneLabel, &lv_font_montserrat_12, LV_PART_MAIN);
  // Battery ----------------------------------------------------------------------
  BattPercentageLabel = lv_label_create(statusbar);
  lv_label_set_text(BattPercentageLabel, "");
  lv_obj_align(BattPercentageLabel, LV_ALIGN_TOP_RIGHT, -28, labelsPositionTopStatusbar);
  lv_obj_set_style_text_font(BattPercentageLabel, &lv_font_montserrat_12, LV_PART_MAIN);
  BattIconLabel = lv_label_create(statusbar);
  lv_label_set_text(BattIconLabel, LV_SYMBOL_BATTERY_EMPTY);
  lv_obj_align(BattIconLabel, LV_ALIGN_TOP_RIGHT, 0, labelsPositionTopStatusbar -1);
  lv_obj_set_style_text_font(BattIconLabel, &lv_font_montserrat_16, LV_PART_MAIN);

}

static bool waitOneLoop = false;
void gui_loop(void) {
  // after the sliding animation ended, we have to wait one cycle of gui_loop() before we can do the recreation of the tabs
  if (waitBeforeActionAfterSlidingAnimationEnded) {
    waitOneLoop = true;
    waitBeforeActionAfterSlidingAnimationEnded = false;
  } else if (waitOneLoop) {
    waitOneLoop = false;
    guis_doAfterSliding(oldTabID, currentTabID);
  };
  // // as alternative, we could wait some milliseconds. But one cycle of gui_loop() works well.
  // if (waitBeforeActionAfterSlidingAnimationEnded) {
  //   if (millis() - waitBeforeActionAfterSlidingAnimationEnded_timerStart >= 5) {
  //     guis_doAfterSliding(oldTabID, currentTabID);
  //     waitBeforeActionAfterSlidingAnimationEnded = false;
  //   }
  // }

  lv_timer_handler();
}

void guis_doTabCreationAtStartup() {
  gui_memoryOptimizer_prepare_startup();

  guis_doAfterSliding(-1, -1);
}

void guis_doAfterSliding(int oldTabID, int newTabID) {
  gui_memoryOptimizer_doAfterSliding_deletionAndCreation(&tabview, oldTabID, newTabID, &panel, &img1, &img2);

  doLogMemoryUsage();
}

void setActiveTab(uint32_t index, lv_anim_enable_t anim_en) {
  // unsigned long startTime = millis();
  if (anim_en == LV_ANIM_ON) {
    lv_tabview_set_act(tabview, index, LV_ANIM_ON);
    // startTime = millis();
    // while (millis() - startTime < 1000) {
    //   lv_timer_handler();
    // }
  } else {
    lv_tabview_set_act(tabview, index, LV_ANIM_OFF);
    // lv_timer_handler();
    // log_memory();
  }
}
