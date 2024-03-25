#include <map>
#include <string>
#include <list>
#include <map>
#include <lvgl.h>
#include "guiRegistry.h"
#include "applicationInternal/gui/guiBase.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "scenes/scene__default.h"

// ------------------------------------------------------------------------------------
// this is a map of the registered_guis that can be accessed by name
std::map<std::string, gui_definition> registered_guis_byName_map;

// ------------------------------------------------------------------------------------


void register_gui(std::string a_name, create_tab_content a_create_tab_content, notify_tab_before_delete a_notify_tab_before_delete) {
  
  if (registered_guis_byName_map.count(a_name) > 0) {
    Serial.printf("ERROR!!!: you cannot register two guis having the same name '%s'\r\n", a_name.c_str());
    return;
  }

  gui_definition new_gui_definition = gui_definition{a_name, a_create_tab_content, a_notify_tab_before_delete};
  
  // put the gui_definition in a map that can be accessed by name
  registered_guis_byName_map[a_name] = new_gui_definition;

  // By default, put all registered guis in the sequence of guis to be shown of the default scene
  // Can be overwritten by scenes to have their own gui_list.
  main_gui_list.insert(main_gui_list.end(), {std::string(a_name)});

}
