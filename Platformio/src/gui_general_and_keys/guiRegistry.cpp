#include <string>
#include <list>
#include <lvgl.h>
#include "guiregistry.h"

// https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work
struct gui_definition {
  init_gui_tab this_init_gui_tab;
  init_gui_pageIndicator this_init_gui_pageIndicator;
};

std::list<gui_definition> registered_guis;

void register_gui(init_gui_tab a_init_gui_tab, init_gui_pageIndicator a_gui_pageIndicator) {
  registered_guis.push_back(gui_definition{a_init_gui_tab, a_gui_pageIndicator});

}

void create_gui_tabs_from_gui_registry(lv_obj_t* tabview) {
  std::list<gui_definition>::iterator it;
  for (it = registered_guis.begin(); it != registered_guis.end(); ++it) {
    it->this_init_gui_tab(tabview);
  }
}

void create_gui_pageIndicators_from_gui_registry(lv_obj_t* panel) {
  std::list<gui_definition>::iterator it;
  for (it = registered_guis.begin(); it != registered_guis.end(); ++it) {
    it->this_init_gui_pageIndicator(panel);
  }
}
