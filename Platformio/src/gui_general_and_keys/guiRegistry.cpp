#include <Arduino.h>
#include <map>
#include <string>
#include <list>
#include <map>
#include <lvgl.h>
#include "guiRegistry.h"
#include "gui_general_and_keys/guiBase.h"

// ------------------------------------------------------------------------------------
// this is a map of the registered_guis that can be accessed by name
std::map<std::string, gui_definition> registered_guis_byName_map;
// This is the list of the guis that we want to be available when swiping. Need not to be all the guis that have been registered, can be only a subset.
// You can swipe through these guis. Will be in the order you place them here in the vector.
std::vector<std::string> list_of_guis_to_be_shown;
std::string currentGUIname = "";

// ------------------------------------------------------------------------------------


void register_gui(std::string a_name, create_tab_content a_create_tab_content, notify_tab_before_delete a_notify_tab_before_delete) {
  
  if (registered_guis_byName_map.count(a_name) > 0) {
    Serial.printf("ERROR!!!: you cannot register two guis having the same name '%s'\r\n", a_name.c_str());
    return;
  }

  gui_definition new_gui_definition = gui_definition{a_name, a_create_tab_content, a_notify_tab_before_delete};
  
  // put the gui_definition in a map that can be accessed by name
  registered_guis_byName_map[a_name] = new_gui_definition;

  // By default, put all registered guis in the sequence of guis to be shown
  // Later we will have scene specific sequences of guis
  list_of_guis_to_be_shown.insert(list_of_guis_to_be_shown.end(), {std::string(a_name)});

}
