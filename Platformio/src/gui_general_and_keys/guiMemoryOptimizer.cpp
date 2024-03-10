#include <Arduino.h>
#include <lvgl.h>
#include "gui_general_and_keys/guiRegistry.h"
#include "gui_general_and_keys/guiBase.h"

struct tab_in_memory {
  lv_obj_t* tab;
  int listIndex;
};
tab_in_memory tabs_in_memory[3] = {{NULL, -1}, {NULL, -1}, {NULL, -1}};
int tabs_in_memory_previous_listIndex[3]= {-1 , -1, -1};

void notify_active_tabs_before_delete() {
  Serial.printf("  Will notify tabs about deletion\r\n");
  std::string nameOfTab;
  for (int index=0; index <= 2; index++) {
    if (tabs_in_memory[index].listIndex == -1) {
      Serial.printf("    Will not notify tab %d about deletion because it does not exist\r\n", index);
      continue;
    }

    nameOfTab = list_of_guis_to_be_shown.at(tabs_in_memory[index].listIndex);
    if (nameOfTab == "") {
      Serial.printf("    Will not notify tab %d about deletion because it is not set\r\n", index);
    } else if (registered_guis_byName_map.count(nameOfTab) == 0) {
      Serial.printf("    Can not notify tab %d about deletion because name \"%s\" was not found in registry\r\n", index, nameOfTab.c_str());
    } else {
      Serial.printf("    Will notify tab %d with name \"%s\" about deletion\r\n", index, nameOfTab.c_str());
      registered_guis_byName_map.at(nameOfTab).this_notify_tab_before_delete();
    }
  }
}

void clear_tabview(lv_obj_t* tabview) {
  if (tabview != NULL) {
    // first remove events for the tabview
    lv_obj_remove_event_cb(tabview, tabview_tab_changed_event_cb);
    lv_obj_remove_event_cb(tabview, tabview_content_is_scrolling_event_cb);
    // delete tabview
    lv_obj_del(tabview);
    tabview = NULL;
  }

  // set struct tabs_in_memory to NULL
  tabs_in_memory[0] = {NULL, -1};
  tabs_in_memory[1] = {NULL, -1};
  tabs_in_memory[2] = {NULL, -1};

}

void clear_panel(lv_obj_t* panel, lv_obj_t* img1, lv_obj_t* img2) {
  if (panel != NULL) {
    lv_obj_del(panel);
    panel = NULL;
  }
  if (img1 != NULL) {
    lv_obj_del(img1);
    img1 = NULL;
  } 
  if (img2 != NULL) {
    lv_obj_del(img2);
    img2 = NULL;
  } 

}

lv_obj_t* create_tabview() {
  // Setup a scrollable tabview for devices and settings ----------------------------------------------------
  lv_obj_t* tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0); // Hide tab labels by setting their height to 0
  #ifdef drawRedBorderAroundMainWidgets
  lv_obj_add_style(tabview, &style_red_border, LV_PART_MAIN);
  #endif
  lv_obj_set_style_bg_color(tabview, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_size(tabview, screenWidth, tabviewHeight);
  lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, tabviewTop);
  return tabview;
}

lv_obj_t* create_panel() {
  // Create a page indicator at the bottom ------------------------------------------------------------------
  lv_obj_t* panel = lv_obj_create(lv_scr_act());
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_CLICKABLE); // This indicator will not be clickable
  lv_obj_set_size(panel, screenWidth, panelHeight);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
  lv_obj_align(panel, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);

  return panel;
}

std::string get_name_of_gui_to_be_shown(int index) {
  if (index == -1) {
    return "";
  } else if (index <= list_of_guis_to_be_shown.size() -1) {
    return list_of_guis_to_be_shown.at(index);
  } else {
    return "";
  }
}

void create_new_tab(lv_obj_t* tabview, uint32_t tabs_in_memory_index) {
  std::string nameOfTab = get_name_of_gui_to_be_shown(tabs_in_memory[tabs_in_memory_index].listIndex);

  if (nameOfTab == "") {
    Serial.printf("    Will not create new tab at index %d because no name was provided\r\n", tabs_in_memory_index);
  } else if (registered_guis_byName_map.count(nameOfTab) == 0) {
    Serial.printf("    Will not create new tab at index %d because name %s was not found in registry\r\n", tabs_in_memory_index, nameOfTab.c_str());
  } else {
    Serial.printf("    Will create tab with name \"%s\" at index %d\r\n", nameOfTab.c_str(), tabs_in_memory_index);
    // create tab and save pointer to tab in tabs_in_memory
    tabs_in_memory[tabs_in_memory_index].tab = lv_tabview_add_tab(tabview, nameOfTab.c_str());
    // let the gui create it's content
    registered_guis_byName_map.at(nameOfTab).this_create_tab_content(tabs_in_memory[tabs_in_memory_index].tab);
  }
}

void doTabCreation_strategyMax3(lv_obj_t* tabview, uint32_t oldTabID, uint32_t newTabID) {
  // create up to three tabs and the content of the tabs
  /*
  example: list_of_guis_to_be_shown: 0 1 2 3 4
  in memory  active
  0 1 -1     0 <- first state, special case - also the initial state
  0 1 2      1
  1 2 3      1
  2 3 4      1
  3 4 -1     1 <- last state, special case
  */

  int tabToBeActivated = -1;
  int oldListIndex = -1;

  if ((oldTabID == -1) && (newTabID == -1)) {
    // This is the initialization after the ESP32 has booted.

    if ((tabs_in_memory_previous_listIndex[0] < list_of_guis_to_be_shown.size()) && (tabs_in_memory_previous_listIndex[0] != -1)) {
      // In gui_memoryOptimizer_prepare_startup, the index of currentGUIname in list_of_guis_to_be_shown was saved, if found.
      // We can resume at this old state.
      oldListIndex = tabs_in_memory_previous_listIndex[0] ; 
      if (oldListIndex == 0) {
        // first state
        Serial.printf("  Startup: will resume where we went to sleep with \"first state\"\r\n");
        tabs_in_memory[0] = {NULL, 0};
        // take care if there is only one gui in list
        tabs_in_memory[1] = {NULL, list_of_guis_to_be_shown.size() >= 2 ? 1 : -1};
        tabs_in_memory[2] = {NULL, -1};
        tabToBeActivated = 0;
      } else if (oldListIndex == list_of_guis_to_be_shown.size() -1) {
        // last state
        Serial.printf("  Startup: will resume where we went to sleep with \"last state\"\r\n");
        tabs_in_memory[0] = {NULL, oldListIndex -1};
        tabs_in_memory[1] = {NULL, oldListIndex};
        tabs_in_memory[2] = {NULL, -1};
        tabToBeActivated = 1;
      } else {
        // any other state
        Serial.printf("  Startup: will resume where we went to sleep with \"state between\"\r\n");
        tabs_in_memory[0] = {NULL, oldListIndex -1};
        tabs_in_memory[1] = {NULL, oldListIndex};
        tabs_in_memory[2] = {NULL, oldListIndex +1};
        tabToBeActivated = 1;
      }
    } else {
      Serial.printf("  Startup: cannot resume old state, so we will show the first tabs from \"list_of_guis_to_be_shown\" as initial state\r\n");
      // take care if there is no gui in list
      tabs_in_memory[0] = {NULL, list_of_guis_to_be_shown.size() != 0 ? 0 : -1};
      // take care if there is only one gui in list
      tabs_in_memory[1] = {NULL, list_of_guis_to_be_shown.size() >= 2 ? 1 : -1};
      tabs_in_memory[2] = {NULL, -1};
      tabToBeActivated = 0;
    }
  } else if (oldTabID > newTabID) {
    // swipe to previous item in list
    Serial.printf("  Will swipe to previous item in list\r\n");
    oldListIndex = tabs_in_memory_previous_listIndex[1];
    if ((oldListIndex == 1)) {
      // next state is the "first state"
      tabs_in_memory[0] = {NULL, 0};
      tabs_in_memory[1] = {NULL, 1};
      tabs_in_memory[2] = {NULL, -1};
      tabToBeActivated = 0;
    } else {
      tabs_in_memory[0] = {NULL, oldListIndex -2};
      tabs_in_memory[1] = {NULL, oldListIndex -1};
      tabs_in_memory[2] = {NULL, oldListIndex};
      tabToBeActivated = 1;
    }
  } else {
    // swipe to next item in list
    Serial.printf("  Will swipe to next item in list\r\n");
    if (tabs_in_memory_previous_listIndex[2] == -1) {
      // last state was the first state
      oldListIndex = tabs_in_memory_previous_listIndex[0]; // is always 0
    } else {
      oldListIndex = tabs_in_memory_previous_listIndex[1];
    }
    if (oldListIndex == list_of_guis_to_be_shown.size() -2) {
      // next state is the "last state"
      tabs_in_memory[0] = {NULL, oldListIndex};
      tabs_in_memory[1] = {NULL, oldListIndex +1};
      tabs_in_memory[2] = {NULL, -1};
      tabToBeActivated = 1;
    } else {
      tabs_in_memory[0] = {NULL, oldListIndex};
      tabs_in_memory[1] = {NULL, oldListIndex +1};
      tabs_in_memory[2] = {NULL, oldListIndex +2};
      tabToBeActivated = 1;
    }
  }

  // create the tabs
  Serial.printf("  Will create tabs. List indices of the three tabs are %d, %d, %d, tab nr %d will be activated\r\n", tabs_in_memory[0].listIndex, tabs_in_memory[1].listIndex, tabs_in_memory[2].listIndex, tabToBeActivated);
  for (int i=0; i<3; i++) {
    create_new_tab(tabview, i);
  }
  
  if (list_of_guis_to_be_shown.size() > 0) {
    std::string nameOfNewActiveTab = list_of_guis_to_be_shown.at(tabs_in_memory[tabToBeActivated].listIndex);
    Serial.printf("  New visible tab is \"%s\"\r\n", nameOfNewActiveTab.c_str());

    // set the tab we swiped to as active
    setActiveTab(tabToBeActivated, LV_ANIM_OFF);
    currentGUIname = nameOfNewActiveTab;
    currentTabID = tabToBeActivated;
  }
}

LV_IMG_DECLARE(gradientLeft);
LV_IMG_DECLARE(gradientRight);

void fillPanelWithPageIndicator_strategyMax3(lv_obj_t* panel, lv_obj_t* img1, lv_obj_t* img2) {
  Serial.printf("  Will fill panel with page indicators\r\n");

  if (list_of_guis_to_be_shown.size() == 0) {
    Serial.printf("    no tab available, so no page indicators\r\n");
    // at least add the style
    lv_obj_add_style(panel, &panel_style, 0);
    #ifdef drawRedBorderAroundMainWidgets
    lv_obj_add_style(panel, &style_red_border, LV_PART_MAIN);
    #endif
    return;
  }

  // This small hidden button enables the page indicator to scroll further
  lv_obj_t* btn = lv_btn_create(panel);
  lv_obj_set_size(btn, 50, lv_pct(100));
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);

/*
  There needs to be two more screen indicators because of their different size (158 for page indicator, 240 for whole tab)
  In some cases they need to have color black, if they are before the first tab or after the last tab.
  In all other cases, they have color "color_primary". See this list:
  example: list_of_guis_to_be_shown: 0 1 2 3 4
  in memory  color         active
  0 1 -1     b p p p       0 <- first state, special case - also the initial state
  0 1 2      b p p p p     1
  1 2 3      p p p p p     1
  2 3 4      p p p p b     1
  3 4 -1     p p p b       1 <- last state, special case
*/    
  // first page indicator before the first tab
  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  if (tabs_in_memory[0].listIndex == 0) {
    lv_obj_set_style_bg_color(btn, lv_color_black(), LV_PART_MAIN);
  } else {
    lv_obj_set_style_bg_color(btn, color_primary,    LV_PART_MAIN);
  }

  // create the panel content for the three guis (or less) which are currently in memory
  std::string nameOfTab;
  for (int i=0; i<3; i++) {
    if (tabs_in_memory[i].listIndex != -1) {
      nameOfTab = get_name_of_gui_to_be_shown(tabs_in_memory[i].listIndex);

      // Create actual (non-clickable) buttons for every tab
      lv_obj_t* btn = lv_btn_create(panel);
      lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_size(btn, 150, lv_pct(100));
      lv_obj_t* label = lv_label_create(btn);
      lv_label_set_text_fmt(label, nameOfTab.c_str());
      lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
      lv_obj_set_style_bg_color(btn, color_primary, LV_PART_MAIN);
    }
  }

  // last page indicator after the last tab
  btn = lv_btn_create(panel);
  lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_size(btn, 150, lv_pct(100));
  //   4 at last position                                                   4 at middle position                                                  only one tab available overall
  if ((tabs_in_memory[2].listIndex == list_of_guis_to_be_shown.size()-1) || (tabs_in_memory[1].listIndex == list_of_guis_to_be_shown.size()-1) || (tabs_in_memory[1].listIndex == -1)) {
    lv_obj_set_style_bg_color(btn, lv_color_black(), LV_PART_MAIN);
  } else {
    lv_obj_set_style_bg_color(btn, color_primary,    LV_PART_MAIN);
  }
  
  // This small hidden button enables the page indicator to scroll further
  btn = lv_btn_create(panel);
  lv_obj_set_size(btn, 50, lv_pct(100));
  lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
  
  // creation of style was moved to init_gui(void)
  // otherwise repeated calls of lv_style_init will lead to a memory leak of about 46 bytes each time
  // https://docs.lvgl.io/8.3/overview/style.html?highlight=lv_style_t#initialize-styles-and-set-get-properties
  lv_obj_add_style(panel, &panel_style, 0);
  #ifdef drawRedBorderAroundMainWidgets
  lv_obj_add_style(panel, &style_red_border, LV_PART_MAIN);
  #endif

  // Make the indicator fade out at the sides using gradient bitmaps
  // Bitmaps are above the buttons and labels
  // don't create it here
  // img1 = lv_img_create(lv_scr_act());
  lv_img_set_src(img1, &gradientLeft);
  lv_obj_align(img1, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_set_size(img1, panelHeight, panelHeight); // stretch the 1-pixel high image to 30px
  #ifdef drawRedBorderAroundMainWidgets
  lv_obj_add_style(img1, &style_red_border, LV_PART_MAIN);
  #endif
  // don't create it here
  // img2 = lv_img_create(lv_scr_act());
  lv_img_set_src(img2, &gradientRight);
  lv_obj_align(img2, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_size(img2, panelHeight, panelHeight);
  #ifdef drawRedBorderAroundMainWidgets
  lv_obj_add_style(img2, &style_red_border, LV_PART_MAIN);
  #endif

}

void gui_memoryOptimizer_prepare_startup() {
  // find index of currentGUIname in list_of_guis_to_be_shown
  for (int i=0; i<list_of_guis_to_be_shown.size(); i++) {
    if (list_of_guis_to_be_shown[i] == currentGUIname) {
      Serial.printf("Startup: found GUI with name \"%s\" in \"list_of_guis_to_be_shown\" at position %d\r\n", currentGUIname.c_str(), i);
      // save position so that "guis_doAfterSliding" can use it
      tabs_in_memory[0].listIndex = i;
    }
  }

}

void gui_memoryOptimizer_doAfterSliding_deletionAndCreation(lv_obj_t** tabview, int oldTabID, int newTabID, lv_obj_t** panel, lv_obj_t** img1, lv_obj_t** img2) {
  // Here the magic for dynamic creation and deletion of lvgl objects happens to keep memory usage low.
  // The next and previous tab must always be available in the tabview, because they can already been seen during the animation.
  // And you always need 3 tabs, otherwise you even could not slide to the next or previous tab.
  // So we always have 3 tabs.
  // After the animation, the tabview and hence all tabs are deleted and recreated.

  bool isInitialization = ((oldTabID == -1) && (newTabID == -1));
  if (isInitialization) {
    Serial.printf("Startup: will initially create the tabs to be shown\r\n");
  } else {
    Serial.printf("Changing from oldTabID %d \"%s\" to newTabID %d \"%s\"\r\n",
      oldTabID, list_of_guis_to_be_shown.at(tabs_in_memory[oldTabID].listIndex).c_str(),
      newTabID, list_of_guis_to_be_shown.at(tabs_in_memory[newTabID].listIndex).c_str());
  }
  
  // save the ids of the tabs we had in memory before
  for (int i=0; i<3; i++) {
    tabs_in_memory_previous_listIndex[i] = tabs_in_memory[i].listIndex;
  }
  
  // the old tabs need to be notified that they will be deleted so that they can persist their state if needed
  if (!isInitialization) notify_active_tabs_before_delete();
  // clear current tabview  
  clear_tabview(*tabview);
  // clear current panel for page indicator
  clear_panel(*panel, *img1, *img2);

  // only optional: delete and create the whole screen. Not necessary.
  // Only used for a test. init_gui_status_bar() would need to be called again at a suitable place, because the status bar would also be deleted.
  // lv_obj_t * oldscr = lv_scr_act();
  // // create new screen
  // lv_obj_t * newscr = lv_obj_create(NULL);
  // // load this new screen
  // lv_scr_load(newscr);
  // lv_obj_del(oldscr);

  // recreate the tabview
  lv_obj_t* newTabview = create_tabview();
  *tabview = newTabview;
  // Create the tabs. Use strategy "3 tabs at maximum" to keep memory usage low.
  // Set the tab we swiped to as active
  doTabCreation_strategyMax3(*tabview, oldTabID, newTabID);

  // Create the panel for the page indicator. Panel itself takes about 2136 bytes for three tabs.
  lv_obj_t* newPanel = create_panel();
  *panel = newPanel;
  *img1 = lv_img_create(lv_scr_act());
  *img2 = lv_img_create(lv_scr_act());
  fillPanelWithPageIndicator_strategyMax3(*panel, *img1, *img2);

  // now, as the correct tab is active, register again the events for the tabview
  lv_obj_add_event_cb(*tabview, tabview_tab_changed_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(lv_tabview_get_content(*tabview), tabview_content_is_scrolling_event_cb, LV_EVENT_SCROLL, NULL);
  // Initialize scroll position of the page indicator
  lv_event_send(lv_tabview_get_content(*tabview), LV_EVENT_SCROLL, NULL);

}
