#include <map>
#include "applicationInternal/keys.h"
#include "applicationInternal/scenes/sceneRegistry.h"
// devices
#include "devices/misc/device_specialCommands.h"
#include "devices/AVreceiver/device_yamahaAmp/device_yamahaAmp.h"
// scenes
#include "scenes/scene_allOff.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"
#include "scenes/scene_chromecast.h"

std::map<char, repeatModes> key_repeatModes_default;
std::map<char, uint16_t> key_commands_short_default;
std::map<char, uint16_t> key_commands_long_default;

void register_scene_defaultKeys(void) {
  key_repeatModes_default = {
                                                                                                             {KEY_OFF,   SHORT            },
    {KEY_STOP,  SHORT            },    {KEY_REWI,  SHORTorLONG      },    {KEY_PLAY,  SHORT            },    {KEY_FORW,  SHORTorLONG      },
    {KEY_CONF,  SHORT            },                                                                          {KEY_INFO,  SHORT            },
                                                         {KEY_UP,    SHORT            },
                      {KEY_LEFT,  SHORT            },    {KEY_OK,    SHORT            },    {KEY_RIGHT, SHORT            },
                                                         {KEY_DOWN,  SHORT            },
    {KEY_BACK,  SHORT            },                                                                          {KEY_SRC,   SHORT            },
    {KEY_VOLUP, SHORT_REPEATED   },                      {KEY_MUTE,  SHORT            },                     {KEY_CHUP,  SHORT            },
    {KEY_VOLDO, SHORT_REPEATED   },                      {KEY_REC,   SHORT            },                     {KEY_CHDOW, SHORT            },
    {KEY_RED,   SHORT            },    {KEY_GREEN, SHORT            },    {KEY_YELLO, SHORT            },    {KEY_BLUE,  SHORT            },
  };
  
  key_commands_short_default = {
                                                                                                             {KEY_OFF,   SCENE_ALLOFF     },
  /*{KEY_STOP,  COMMAND_UNKNOWN  },    {KEY_REWI,  COMMAND_UNKNOWN  },    {KEY_PLAY,  COMMAND_UNKNOWN  },    {KEY_FORW,  COMMAND_UNKNOWN  },*/
  /*{KEY_CONF,  COMMAND_UNKNOWN  },                                                                          {KEY_INFO,  COMMAND_UNKNOWN  },*/
                                                     /*  {KEY_UP,    COMMAND_UNKNOWN  },*/
                   /* {KEY_LEFT,  COMMAND_UNKNOWN  },    {KEY_OK,    COMMAND_UNKNOWN  },    {KEY_RIGHT, COMMAND_UNKNOWN  },*/
                                                     /*  {KEY_DOWN,  COMMAND_UNKNOWN  },*/
  /*{KEY_BACK,  COMMAND_UNKNOWN  },                                                                          {KEY_SRC,   COMMAND_UNKNOWN  },*/
    {KEY_VOLUP, YAMAHA_VOL_PLUS  },                      {KEY_MUTE,  YAMAHA_MUTE_TOGGLE},                  /*{KEY_CHUP,  COMMAND_UNKNOWN  },*/
    {KEY_VOLDO, YAMAHA_VOL_MINUS },                   /* {KEY_REC,   COMMAND_UNKNOWN  },*/                 /*{KEY_CHDOW, COMMAND_UNKNOWN  },*/
    {KEY_RED,   SCENE_TV         },    {KEY_GREEN, SCENE_FIRETV     },    {KEY_YELLO, SCENE_CHROMECAST },    {KEY_BLUE,  YAMAHA_STANDARD  },
  };
  
  key_commands_long_default = {
  
  
  };

}
