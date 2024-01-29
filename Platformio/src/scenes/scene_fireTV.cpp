#include <map>
#include "gui_general_and_keys/keys.h"
#include "device_samsungTV/device_samsungTV.h"
#include "device_yamahaAmp/device_yamahaAmp.h"
#include "scenes/sceneRegistry.h"
#include "scenes/scene_allOff.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"
#include "scenes/scene_chromecast.h"
#include "commandHandler.h"

std::map<char, repeatModes> key_repeatModes_fireTV {
                                                                             {'o', SHORT         },
  {'=', SHORT         },   {'<', SHORTorLONG   },   {'p', SHORT         },   {'>', SHORTorLONG   },
  {'c', SHORT         },                                                     {'i', SHORT         },
                                         {'u', SHORT         },
                {'l', SHORT         },   {'k', SHORT         },   {'r', SHORT         },
                                         {'d', SHORT         },
  {'b', SHORT         },                                                     {'s', SHORT         },
  {'+', SHORT_REPEATED},                 {'m', SHORT         },              {'^', SHORT         },
  {'-', SHORT_REPEATED},                 {'e', SHORT         },              {'v', SHORT         },
  {'1', SHORT         },   {'2', SHORT         },   {'3', SHORT          },  {'4', SHORT         },
};

std::map<char, std::string> key_commands_short_fireTV {
                                                                                               {'o', SCENE_ALLOFF},
/*{'=', KEYBOARD_PLAYPAUSE},*/    {'<', KEYBOARD_REWIND},      {'p', KEYBOARD_PLAYPAUSE},      {'>', KEYBOARD_FASTFORWARD},
  {'c', KEYBOARD_HOME},                                                                        {'i', KEYBOARD_MENU},
                                                {'u', KEYBOARD_UP},
                    {'l', KEYBOARD_LEFT},       {'k', KEYBOARD_SELECT},     {'r', KEYBOARD_RIGHT},
                                                {'d', KEYBOARD_DOWN},
  /* {'b', }, */                                                                               {'s', KEYBOARD_BACK},
  {'+', YAMAHA_VOL_PLUS},                       {'m', YAMAHA_MUTE_TOGGLE},                     {'^', SAMSUNG_CHANNEL_UP},
  {'-', YAMAHA_VOL_MINUS},                      /* {'e', }, */                                 {'v', SAMSUNG_CHANNEL_DOWN},
  {'1', SCENE_TV},                {'2', SCENE_FIRETV},         {'3', SCENE_CHROMECAST},        {'4', YAMAHA_STANDARD},
};

std::map<char, std::string> key_commands_long_fireTV {
  {'<', KEYBOARD_REWIND_LONG},
  {'>', KEYBOARD_FASTFORWARD_LONG},
};

void scene_start_sequence_fireTV(void) {
  executeCommand(SAMSUNG_POWER_ON);
  delay(500);
  executeCommand(YAMAHA_POWER_ON);
  delay(1500);
  executeCommand(YAMAHA_INPUT_DTV);
  delay(3000);
  executeCommand(SAMSUNG_INPUT_HDMI_2);
  delay(100);
  
  executeCommand(KEYBOARD_HOME);
  delay(500);
  executeCommand(KEYBOARD_HOME);

}

void scene_end_sequence_fireTV(void) {
  // you cannot power off FireTV, but at least you can stop the currently running app
  executeCommand(KEYBOARD_HOME);
  delay(500);
  executeCommand(KEYBOARD_HOME);

}

std::string scene_name_fireTV = "Fire TV";

void register_scene_fireTV(void){
  register_scene(
    scene_name_fireTV,
    & scene_start_sequence_fireTV,
    & scene_end_sequence_fireTV,
    & key_repeatModes_fireTV,
    & key_commands_short_fireTV,
    & key_commands_long_fireTV);

  commands[SCENE_FIRETV]     = makeCommandData(SCENE, {scene_name_fireTV});
}
