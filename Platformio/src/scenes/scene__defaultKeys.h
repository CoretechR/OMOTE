#pragma once

#include <string>
#include <map>
#include "applicationInternal/keys.h"

extern std::map<char, repeatModes> key_repeatModes_default;
extern std::map<char, uint16_t> key_commands_short_default;
extern std::map<char, uint16_t> key_commands_long_default;
void register_scene_defaultKeys(void);
