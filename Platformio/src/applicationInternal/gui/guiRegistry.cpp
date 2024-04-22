#include <map>
#include <string>
#include <list>
#include <map>
#include <lvgl.h>
#include "guiRegistry.h"
#include "applicationInternal/gui/guiBase.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "scenes/scene__default.h"

// ------------------------------------------------------------------------------------
// this is a map of the registered_guis that can be accessed by name
std::map<std::string, gui_definition> registered_guis_byName_map;

// ------------------------------------------------------------------------------------


void register_gui(
  std::string a_name,
  create_tab_content a_create_tab_content,
  notify_tab_before_delete a_notify_tab_before_delete,
  gui_setKeys a_gui_setKeys,
  key_repeatModes a_key_repeatModes,
  key_commands_short a_key_commands_short,
  key_commands_long a_key_commands_long
  ) {
  
  if (registered_guis_byName_map.count(a_name) > 0) {
    Serial.printf("ERROR!!!: you cannot register two guis having the same name '%s'\r\n", a_name.c_str());
    return;
  }

  gui_definition new_gui_definition = gui_definition{
    a_name,
    a_create_tab_content,
    a_notify_tab_before_delete,
    a_gui_setKeys,
    a_key_repeatModes,
    a_key_commands_short,
    a_key_commands_long
  };
  
  // put the gui_definition in a map that can be accessed by name
  registered_guis_byName_map[a_name] = new_gui_definition;

  // By default, put all registered guis in the sequence of guis to be shown of the default scene
  // Can be overwritten by scenes to have their own gui_list.
  main_gui_list.insert(main_gui_list.end(), {std::string(a_name)});

  // Whenever a new gui is registered, a new gui command could have been defined.
  // But this new gui command could have been already been used before in the key definition of another gui. The command at this time was 0, which is undefined.
  // So we have to set the keys again for all guis that have been registered before.
  // Loop over all registered guis and call setKeys()
  for (std::map<std::string, gui_definition>::iterator it = registered_guis_byName_map.begin(); it != registered_guis_byName_map.end(); ++it) {
    if (it->second.this_gui_setKeys != NULL) {
      it->second.this_gui_setKeys();
    }
  }

}
