#include <map>
#include <string>
#include "device_samsungTV/device_samsungTV.h"
#include "device_yamahaAmp/device_yamahaAmp.h"
#include "scenes/sceneRegistry.h"
#include "scenes/scene_allOff.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"
#include "scenes/scene_chromecast.h"
#include "commandHandler.h"

std::map<char, repeatModes> key_repeatModes_default {
                                                                                              {'o', SHORT         },
  {'=', SHORT         },          {'<', SHORTorLONG   },       {'p', SHORT         },         {'>', SHORTorLONG   },
  {'c', SHORT         },                                                                      {'i', SHORT         },
                                                {'u', SHORT         },
                    {'l', SHORT         },      {'k', SHORT         },      {'r', SHORT         },
                                                {'d', SHORT         },
  {'b', SHORT         },                                                                      {'s', SHORT         },
  {'+', SHORT_REPEATED},                        {'m', SHORT         },                        {'^', SHORT         },
  {'-', SHORT_REPEATED},                        {'e', SHORT         },                        {'v', SHORT         },
  {'1', SHORT         },          {'2', SHORT         },       {'3', SHORT          },        {'4', SHORT         },
};

std::map<char, std::string> key_commands_short_default {
                                                                                               {'o', SCENE_ALLOFF},
/*{'=', COMMAND_UNKNOWN},*/     /*{'<', COMMAND_UNKNOWN},*/  /*{'p', COMMAND_UNKNOWN},*/     /*{'>', COMMAND_UNKNOWN},*/
/*{'c', COMMAND_UNKNOWN},  */                                                                /*{'i', COMMAND_UNKNOWN},*/
                                              /*{'u', COMMAND_UNKNOWN},*/
                  /*{'l', COMMAND_UNKNOWN},*/ /*{'k', COMMAND_UNKNOWN},*/ /*{'r', COMMAND_UNKNOWN},*/
                                              /*{'d', COMMAND_UNKNOWN},*/
/* {'b', COMMAND_UNKNOWN},*/                                                                 /*{'s', COMMAND_UNKNOWN},*/
  {'+', YAMAHA_VOL_PLUS},                       {'m', YAMAHA_MUTE_TOGGLE},                   /*{'^', COMMAND_UNKNOWN},*/
  {'-', YAMAHA_VOL_MINUS},                    /*{'e', COMMAND_UNKNOWN},*/                    /*{'v', COMMAND_UNKNOWN},*/
  {'1', SCENE_TV},                {'2', SCENE_FIRETV},         {'3', SCENE_CHROMECAST},        {'4', YAMAHA_STANDARD},
};

std::map<char, std::string> key_commands_long_default {


};

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

std::string get_command_short(std::string sceneName, char keyChar) {
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

std::string get_command_long(std::string sceneName, char keyChar) {
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

