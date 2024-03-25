#include <map>
#include "scenes/scene_TV.h"
#include "applicationInternal/keys.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
// devices
#include "devices/TV/device_samsungTV/device_samsungTV.h"
#include "devices/AVreceiver/device_yamahaAmp/device_yamahaAmp.h"
#include "applicationInternal/commandHandler.h"

uint16_t SCENE_TV          ; //"Scene_tv"

std::map<char, repeatModes> key_repeatModes_TV;
std::map<char, uint16_t> key_commands_short_TV;
std::map<char, uint16_t> key_commands_long_TV;

void scene_setKeys_TV() {
  key_repeatModes_TV = {
  
    {KEY_STOP,  SHORT_REPEATED   },    {KEY_REWI,  SHORT            },    {KEY_PLAY,  SHORT            },    {KEY_FORW,  SHORT_REPEATED   },
    {KEY_CONF,  SHORT            },                                                                          {KEY_INFO,  SHORT            },
                                                         {KEY_UP,    SHORT_REPEATED   },
                      {KEY_LEFT,  SHORT_REPEATED   },    {KEY_OK,    SHORT            },    {KEY_RIGHT, SHORT_REPEATED  },
                                                         {KEY_DOWN,  SHORT_REPEATED   },
                                                                                                             {KEY_SRC,   SHORT            },
                                                                                                             {KEY_CHUP,  SHORT            },
                                                                                                             {KEY_CHDOW, SHORT            },
  
  };
  
  key_commands_short_TV = {
  
    {KEY_STOP,  SAMSUNG_REWIND   },    {KEY_REWI,  SAMSUNG_PAUSE    },    {KEY_PLAY,  SAMSUNG_PLAY     },    {KEY_FORW,  SAMSUNG_FASTFORWARD},
    {KEY_CONF,  SAMSUNG_GUIDE    },                                                                          {KEY_INFO,  SAMSUNG_MENU     },
                                                         {KEY_UP,    SAMSUNG_UP       },
                      {KEY_LEFT,  SAMSUNG_LEFT    },     {KEY_OK,    SAMSUNG_SELECT   },    {KEY_RIGHT, SAMSUNG_RIGHT    },
                                                         {KEY_DOWN,  SAMSUNG_DOWN     },
                                                                                                             {KEY_SRC,   SAMSUNG_EXIT     },
                                                                                                             {KEY_CHUP,  SAMSUNG_CHANNEL_UP},
                                                                                                             {KEY_CHDOW, SAMSUNG_CHANNEL_DOWN},
  
  };
  
  key_commands_long_TV = {
  
  
  };

}

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

void register_scene_TV(void) {
  register_command(&SCENE_TV         , makeCommandData(SCENE, {scene_name_TV}));

  register_scene(
    scene_name_TV,
    & scene_setKeys_TV,
    & scene_start_sequence_TV,
    & scene_end_sequence_TV,
    & key_repeatModes_TV,
    & key_commands_short_TV,
    & key_commands_long_TV);
}
