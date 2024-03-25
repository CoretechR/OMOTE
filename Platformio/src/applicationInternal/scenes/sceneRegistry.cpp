#include <map>
#include <stdexcept>
#include "applicationInternal/commandHandler.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
// scenes
#include "scenes/scene__defaultKeys.h"

// If useSceneGUIlist == true, then a scene is active and we are not in the main_gui_list (with the scene selector as first gui).
//   In that case, we try to use the scene specific gui list, if the scene defined one.
// If useSceneGUIlist == false, then we are in the main_gui_list, either if a scene is active or not.
bool useSceneGUIlist = false;

// https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work
struct scene_definition {
  scene_setKeys this_scene_setKeys;
  scene_start_sequence this_scene_start_sequence;
  scene_end_sequence this_scene_end_sequence;
  key_repeatModes this_key_repeatModes;
  key_commands_short this_key_commands_short;
  key_commands_long this_key_commands_long;
  gui_list this_gui_list;
  uint16_t this_activate_scene_command;
};

std::map<std::string, scene_definition> registered_scenes;
t_scene_list scenes_on_sceneSelectionGUI;

void register_scene(
  std::string a_scene_name,
  scene_setKeys a_scene_setKeys,
  scene_start_sequence a_scene_start_sequence,
  scene_end_sequence a_scene_end_sequence,
  key_repeatModes a_key_repeatModes,
  key_commands_short a_key_commands_short,
  key_commands_long a_key_commands_long,
  gui_list a_gui_list,
  uint16_t a_activate_scene_command) {

  // put the scene_definition in a map that can be accessed by name
  registered_scenes[a_scene_name] = scene_definition{
    a_scene_setKeys,
    a_scene_start_sequence,
    a_scene_end_sequence,
    a_key_repeatModes,
    a_key_commands_short,
    a_key_commands_long,
    a_gui_list,
    a_activate_scene_command
  };

  // Additionally, put all registered scenes in a sequence of scenes to be shown in the sceneSelection gui.
  // Exactly in the order they have been registered.
  // Can be overwritten in main.cpp
  scenes_on_sceneSelectionGUI.insert(scenes_on_sceneSelectionGUI.end(), {std::string(a_scene_name)});

  // Whenever a new scene is registered, normally a new scene command has been defined immediately before (e.g. see register_scene_TV()).
  // But this new scene command could have been already been used before in the key definition of another scene. The command at this time was 0, which is undefined.
  // So we have to set the keys again for all scenes that have been registered before.
  // 1. set again the defaultKeys
  register_scene_defaultKeys();
  // 2. loop over all registered scenes and call setKeys()
  for (std::map<std::string, scene_definition>::iterator it = registered_scenes.begin(); it != registered_scenes.end(); ++it) {
    it->second.this_scene_setKeys();
  }

}

bool sceneExists(std::string sceneName) {
  return (registered_scenes.count(sceneName) > 0);
}

void scene_start_sequence_from_registry(std::string sceneName) {
  try {
    registered_scenes.at(sceneName).this_scene_start_sequence();
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("scene_start_sequence_from_registry: internal error, sceneName not registered\r\n");
  }
}

void scene_end_sequence_from_registry(std::string sceneName) {
  try {
    registered_scenes.at(sceneName).this_scene_end_sequence();
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("scene_end_sequence_from_registry: internal error, sceneName not registered\r\n");
  }
}

repeatModes get_key_repeatMode(std::string sceneName, char keyChar) {
  try {
    // look if the map of the current scene has a definition for it
    if ((registered_scenes.count(sceneName) > 0) && (registered_scenes.at(sceneName).this_key_repeatModes->count(keyChar) > 0)) {
      // Serial.printf("get_key_repeatMode: will use key from scene %s\r\n", sceneName.c_str());
      return registered_scenes.at(sceneName).this_key_repeatModes->at(keyChar);

    // look if there is a default definition
    } else if (key_repeatModes_default.count(keyChar) > 0) {
      // Serial.printf("get_key_repeatMode: will use default key\r\n");
      return key_repeatModes_default.at(keyChar);

    // no key definition found
    } else {
      // Serial.printf("get_key_repeatMode: WARNING no key definition found\r\n");
      return REPEAT_MODE_UNKNOWN;
    }
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("get_key_repeatMode: internal error, sceneName not registered\r\n");
    return REPEAT_MODE_UNKNOWN;
  }
}

uint16_t get_command_short(std::string sceneName, char keyChar) {
  try {
    // look if the map of the current scene has a definition for it
    if ((registered_scenes.count(sceneName) > 0) && (registered_scenes.at(sceneName).this_key_commands_short->count(keyChar) > 0)) {
      // Serial.printf("get_command_short: will use key from scene %s\r\n", sceneName.c_str());
      return registered_scenes.at(sceneName).this_key_commands_short->at(keyChar);
    
    // look if there is a default definition
    } else if (key_commands_short_default.count(keyChar) > 0) {
      // Serial.printf("get_command_short: will use default key\r\n");
      return key_commands_short_default.at(keyChar);
    
    // no key definition found
    } else {
      // Serial.printf("get_command_short: WARNING no key definition found\r\n");
      return COMMAND_UNKNOWN;
    }
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("get_command_short: internal error, sceneName not registered\r\n");
    return COMMAND_UNKNOWN;
  }

}

uint16_t get_command_long(std::string sceneName, char keyChar) {
  try {
    // look if the map of the current scene has a definition for it
    if ((registered_scenes.count(sceneName) > 0) && (registered_scenes.at(sceneName).this_key_commands_long->count(keyChar) > 0)) {
      // Serial.printf("get_command_long: will use key from scene %s\r\n", sceneName.c_str());
      return registered_scenes.at(sceneName).this_key_commands_long->at(keyChar);
    
    // look if there is a default definition
    } else if (key_commands_long_default.count(keyChar) > 0) {
      // Serial.printf("get_command_long: will use default key\r\n");
      return key_commands_long_default.at(keyChar);
    
    // no key definition found
    } else {
      // Serial.printf("get_command_long: WARNING no key definition found\r\n");
      return COMMAND_UNKNOWN;
    }
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("get_command_long: internal error, sceneName not registered\r\n");
    return COMMAND_UNKNOWN;
  }

}

gui_list get_gui_list(std::string sceneName) {
  try {
    // If useSceneGUIlist == true, then a scene is active and we are not in the main_gui_list (with the scene selector as first gui).
    //   In that case, we try to use the scene specific gui list, if the scene defined one.
    // If useSceneGUIlist == false, then we are in the main_gui_list, either if a scene is active or not.

    #if (USE_SCENE_SPECIFIC_GUI_LIST != 0)
    // look if the current scene has a definition for a gui list
    if (useSceneGUIlist &&
        (registered_scenes.count(sceneName) > 0) && (registered_scenes.at(sceneName).this_gui_list != NULL)) {
      // Serial.printf("get_gui_list: will use gui_list from scene %s\r\n", sceneName.c_str());
      return registered_scenes.at(sceneName).this_gui_list;
    
    // if there is no scene specific gui list, simply return the main_gui_list
    } else {
      // Serial.printf("get_gui_list: will use main_gui_list\r\n");
      return &main_gui_list;
    
    }
    #else
    // never use scene specific gui list
    return &main_gui_list;
    #endif
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("get_gui_list: internal error, sceneName not registered\r\n");
    return NULL;
  }

}

uint16_t get_activate_scene_command(std::string sceneName) {
  try {
    // look if the current scene is known
    if ((registered_scenes.count(sceneName) > 0)) {
      // Serial.printf("get_activate_scene_command: will use activate_scene_command from scene %s\r\n", sceneName.c_str());
      return registered_scenes.at(sceneName).this_activate_scene_command;
    
    // if the scene is not know, simply return 0
    } else {
      // Serial.printf("get_activate_scene_command: will return 0\r\n");
      return 0;
    
    }
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("get_activate_scene_command: internal error, sceneName not registered\r\n");
    return 0;
  }

}

scene_list get_scenes_on_sceneSelectionGUI() {
  return &scenes_on_sceneSelectionGUI;
}

void set_scenes_on_sceneSelectionGUI(t_scene_list a_scene_list) {
  scenes_on_sceneSelectionGUI.clear();
  for (int i=0; i<a_scene_list.size(); i++) {
    scenes_on_sceneSelectionGUI.insert(scenes_on_sceneSelectionGUI.end(), a_scene_list.at(i));
  }

}


char KEY_OFF    = 'o';
char KEY_STOP   = '=';
char KEY_REWI   = '<';
char KEY_PLAY   = 'p';
char KEY_FORW   = '>';
char KEY_CONF   = 'c';
char KEY_INFO   = 'i';
char KEY_UP     = 'u';
char KEY_DOWN   = 'd';
char KEY_LEFT   = 'l';
char KEY_RIGHT  = 'r';
char KEY_OK     = 'k';
char KEY_BACK   = 'b';
char KEY_SRC    = 's';
char KEY_VOLUP  = '+';
char KEY_VOLDO  = '-';
char KEY_MUTE   = 'm';
char KEY_REC    = 'e';
char KEY_CHUP   = '^';
char KEY_CHDOW  = 'v';
char KEY_RED    = '1';
char KEY_GREEN  = '2';
char KEY_YELLO  = '3';
char KEY_BLUE   = '4';
