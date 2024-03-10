#include <map>
#include <stdexcept>
#include "devices/misc/device_specialCommands.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
// scenes
#include "scenes/scene__defaultKeys.h"

// https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work
struct scene_definition {
  scene_start_sequence this_scene_start_sequence;
  scene_end_sequence this_scene_end_sequence;
  key_repeatModes this_key_repeatModes;
  key_commands_short this_key_commands_short;
  key_commands_long this_key_commands_long;
};

std::map<std::string, scene_definition> registered_scenes;

void register_scene(
  std::string a_scene_name,
  scene_start_sequence a_scene_start_sequence,
  scene_end_sequence a_scene_end_sequence,
  key_repeatModes a_key_repeatModes,
  key_commands_short a_key_commands_short,
  key_commands_long a_key_commands_long) {

  registered_scenes[a_scene_name] = scene_definition{
    a_scene_start_sequence,
    a_scene_end_sequence,
    a_key_repeatModes,
    a_key_commands_short,
    a_key_commands_long
  };

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
      Serial.printf("get_command_short: will use key from scene %s\r\n", sceneName.c_str());
      return registered_scenes.at(sceneName).this_key_commands_short->at(keyChar);
    
    // look if there is a default definition
    } else if (key_commands_short_default.count(keyChar) > 0) {
      Serial.printf("get_command_short: will use default key\r\n");
      return key_commands_short_default.at(keyChar);
    
    // no key definition found
    } else {
      Serial.printf("get_command_short: WARNING no key definition found\r\n");
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
      Serial.printf("get_command_long: will use key from scene %s\r\n", sceneName.c_str());
      return registered_scenes.at(sceneName).this_key_commands_long->at(keyChar);
    
    // look if there is a default definition
    } else if (key_commands_long_default.count(keyChar) > 0) {
      Serial.printf("get_command_long: will use default key\r\n");
      return key_commands_long_default.at(keyChar);
    
    // no key definition found
    } else {
      Serial.printf("get_command_long: WARNING no key definition found\r\n");
      return COMMAND_UNKNOWN;
    }
  }
  catch (const std::out_of_range& oor) {
    Serial.printf("get_command_long: internal error, sceneName not registered\r\n");
    return COMMAND_UNKNOWN;
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
