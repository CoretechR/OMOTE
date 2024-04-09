#pragma once

#include <lvgl.h>

// used by memoryUsage.cpp
extern lv_obj_t* MemoryUsageLabel;
// used by guiStatusUpdate.cpp
extern lv_obj_t* BluetoothLabel;
extern lv_obj_t* BattPercentageLabel;
extern lv_obj_t* BattIconLabel;
// used by sceneHandler.cpp
extern lv_obj_t* SceneLabel;
// used by guiMemoryOptimizer.cpp
extern lv_style_t panel_style;
extern int tabviewTop;
extern int tabviewHeight;
extern int panelHeight;
extern uint32_t currentTabID;
// used by almost all gui_*.cpp
extern lv_color_t color_primary;

//#define drawRedBorderAroundMainWidgets
#ifdef drawRedBorderAroundMainWidgets
extern lv_style_t style_red_border;
#endif

// used by main.cpp and sceneHandler.cpp
void init_gui(void);
void gui_loop(void);
// used by guiMemoryOptimizer.cpp
void tabview_content_is_scrolling_event_cb(lv_event_t* e);
void tabview_tab_changed_event_cb(lv_event_t* e);
void sceneLabel_or_pageIndicator_event_cb(lv_event_t* e);
void pageIndicator_navigate_event_cb(lv_event_t* e);
void setActiveTab(uint32_t index, lv_anim_enable_t anim_en, bool send_tab_changed_event = false);
// used by memoryUsage.cpp
void showMemoryUsageBar(bool showBar);
// used by commandHandler to show WiFi status
void showWiFiConnected(bool connected);

void guis_doAfterSliding(int oldTabID, int newTabID, bool newGuiList);
