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

std::map<char, repeatModes> key_repeatModes_chromecast {
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

std::map<char, std::string> key_commands_short_chromecast {
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

std::map<char, std::string> key_commands_long_chromecast {
  {'<', KEYBOARD_REWIND_LONG},
  {'>', KEYBOARD_FASTFORWARD_LONG},
};

void scene_start_sequence_chromecast(void) {
  executeCommand(SAMSUNG_POWER_ON);
  delay(500);
  executeCommand(YAMAHA_POWER_ON);
  delay(1500);
  executeCommand(YAMAHA_INPUT_DVD);
  delay(3000);
  executeCommand(SAMSUNG_INPUT_HDMI_1);

}

void scene_end_sequence_chromecast(void) {

}

std::string scene_name_chromecast = "Chromecast";

void register_scene_chromecast(void){
  register_scene(
    scene_name_chromecast,
    & scene_start_sequence_chromecast,
    & scene_end_sequence_chromecast,
    & key_repeatModes_chromecast,
    & key_commands_short_chromecast,
    & key_commands_long_chromecast);

  commands[SCENE_CHROMECAST] = makeCommandData(SCENE, {scene_name_chromecast});
}
