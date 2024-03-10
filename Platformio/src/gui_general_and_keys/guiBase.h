#ifndef __GUIBASE_H__
#define __GUIBASE_H__

#include <lvgl.h>
#include "../hardware/tft.h"

// -----------------------
// https://docs.lvgl.io/8.3/porting/display.html?highlight=lv_disp_draw_buf_init#buffering-modes
// With two buffers, the rendering and refreshing of the display become parallel operations
// Second buffer needs 15.360 bytes more memory in heap.
#define useTwoBuffersForlvgl

// LVGL declarations
LV_IMG_DECLARE(high_brightness);
LV_IMG_DECLARE(low_brightness);
extern lv_obj_t* MemoryUsageLabel;
extern lv_obj_t* WifiLabel;
extern lv_obj_t* BluetoothLabel;
extern lv_obj_t* SceneLabel;
extern lv_obj_t* BattPercentageLabel;
extern lv_obj_t* BattIconLabel;

//#define drawRedBorderAroundMainWidgets
#ifdef drawRedBorderAroundMainWidgets
extern lv_style_t style_red_border;
#endif
extern lv_style_t panel_style;
extern int tabviewTop;
extern int tabviewHeight;
extern int panelHeight;

extern lv_color_t color_primary;

extern uint32_t currentTabID;

void init_gui(void);
void gui_loop(void);

void tabview_content_is_scrolling_event_cb(lv_event_t* e);
void tabview_tab_changed_event_cb(lv_event_t* e);

void guis_doTabCreationAtStartup();
void guis_doAfterSliding(int oldTabID, int newTabID);
void setActiveTab(uint32_t index, lv_anim_enable_t anim_en);

void showMemoryUsageBar(bool showBar);

#endif /*__GUIBASE_H__*/
