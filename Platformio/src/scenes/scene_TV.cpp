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

std::map<char, repeatModes> key_repeatModes_TV {

  {'=', SHORT_REPEATED},          {'<', SHORT   },             {'p', SHORT         },         {'>', SHORT_REPEATED   },
  {'c', SHORT         },                                                                      {'i', SHORT         },
                                                {'u', SHORT_REPEATED},
                    {'l', SHORT_REPEATED},      {'k', SHORT},               {'r', SHORT_REPEATED},
                                                {'d', SHORT_REPEATED},
                                                                                              {'s', SHORT         },
                                                                                              {'^', SHORT         },
                                                                                              {'v', SHORT         },

};

std::map<char, std::string> key_commands_short_TV {

  {'=', SAMSUNG_REWIND},          {'<', SAMSUNG_PAUSE},        {'p', SAMSUNG_PLAY},            {'>', SAMSUNG_FASTFORWARD},
  {'c', SAMSUNG_GUIDE},                                                                        {'i', SAMSUNG_MENU},
                                                {'u', SAMSUNG_UP},
                    {'l', SAMSUNG_LEFT},        {'k', SAMSUNG_SELECT},      {'r', SAMSUNG_RIGHT},
                                                {'d', SAMSUNG_DOWN},
                                                                                               {'s', SAMSUNG_EXIT},
                                                                                               {'^', SAMSUNG_CHANNEL_UP},
                                                                                               {'v', SAMSUNG_CHANNEL_DOWN},

};

std::map<char, std::string> key_commands_long_TV {


};

void scene_start_sequence_TV(void) {
  executeCommand(SAMSUNG_POWER_ON);
  delay(500);
  executeCommand(YAMAHA_POWER_ON);
  delay(1500);
  executeCommand(YAMAHA_INPUT_DVD);
  delay(3000);
  executeCommand(SAMSUNG_INPUT_TV);

}

void scene_end_sequence_TV(void) {

}

std::string scene_name_TV = "TV";

void register_scene_TV(void){
  register_scene(
    scene_name_TV,
    & scene_start_sequence_TV,
    & scene_end_sequence_TV,
    & key_repeatModes_TV,
    & key_commands_short_TV,
    & key_commands_long_TV);

  commands[SCENE_TV]         = makeCommandData(SCENE, {scene_name_TV});
}
