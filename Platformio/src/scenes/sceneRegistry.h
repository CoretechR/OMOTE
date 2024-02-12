#ifndef __SCENEREGISTRY_H__
#define __SCENEREGISTRY_H__

#include <map>
#include "gui_general_and_keys/keys.h"

typedef void (*scene_start_sequence)(void);
typedef void (*scene_end_sequence)(void);
typedef std::map<char, repeatModes> *key_repeatModes;
typedef std::map<char, std::string> *key_commands_short;
typedef std::map<char, std::string> *key_commands_long;

void register_scene(
  std::string a_scene_name,
  scene_start_sequence a_scene_start_sequence,
  scene_end_sequence a_scene_end_sequence,
  key_repeatModes a_key_repeatModes,
  key_commands_short a_key_commands_short,
  key_commands_long a_key_commands_long);

#define COMMAND_UNKNOWN "command_unknown"

bool sceneExists(std::string sceneName);
void scene_start_sequence_from_registry(std::string sceneName);
void scene_end_sequence_from_registry(std::string sceneName);
repeatModes get_key_repeatMode(std::string sceneName, char keyChar);
std::string get_command_short(std::string sceneName, char keyChar);
std::string get_command_long(std::string sceneName, char keyChar);

#endif /*__SCENEREGISTRY_H__*/
