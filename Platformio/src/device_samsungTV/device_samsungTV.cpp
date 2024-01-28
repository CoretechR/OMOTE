#include "commandHandler.h"
#include "device_samsungTV/device_samsungTV.h"

void register_device_samsung() {
  // both GC and SAMSUNG work well

  // https://github.com/natcl/studioimaginaire/blob/master/arduino_remote/ircodes.py
  commands[SAMSUNG_POWER_TOGGLE]      = makeCommandData(IR_SAMSUNG, {"0xE0E040BF"});
  commands[SAMSUNG_SOURCE]            = makeCommandData(IR_SAMSUNG, {"0xE0E0807F"});
  commands[SAMSUNG_HDMI]              = makeCommandData(IR_SAMSUNG, {"0xE0E0D12E"});
  commands[SAMSUNG_NUM_1]             = makeCommandData(IR_SAMSUNG, {"0xE0E020DF"});
  commands[SAMSUNG_NUM_2]             = makeCommandData(IR_SAMSUNG, {"0xE0E0A05F"});
  commands[SAMSUNG_NUM_3]             = makeCommandData(IR_SAMSUNG, {"0xE0E0609F"});
  commands[SAMSUNG_NUM_4]             = makeCommandData(IR_SAMSUNG, {"0xE0E010EF"});
  commands[SAMSUNG_NUM_5]             = makeCommandData(IR_SAMSUNG, {"0xE0E0906F"});
  commands[SAMSUNG_NUM_6]             = makeCommandData(IR_SAMSUNG, {"0xE0E050AF"});
  commands[SAMSUNG_NUM_7]             = makeCommandData(IR_SAMSUNG, {"0xE0E030CF"});
  commands[SAMSUNG_NUM_8]             = makeCommandData(IR_SAMSUNG, {"0xE0E0B04F"});
  commands[SAMSUNG_NUM_9]             = makeCommandData(IR_SAMSUNG, {"0xE0E0708F"});
  commands[SAMSUNG_NUM_0]             = makeCommandData(IR_SAMSUNG, {"0xE0E08877"});
  commands[SAMSUNG_TTXMIX]            = makeCommandData(IR_SAMSUNG, {"0xE0E034CB"});
  commands[SAMSUNG_PRECH]             = makeCommandData(IR_SAMSUNG, {"0xE0E0C837"});
  commands[SAMSUNG_VOL_MINUS]         = makeCommandData(IR_SAMSUNG, {"0xE0E0D02F"});
  commands[SAMSUNG_VOL_PLUS]          = makeCommandData(IR_SAMSUNG, {"0xE0E0E01F"});
  commands[SAMSUNG_MUTE_TOGGLE]       = makeCommandData(IR_SAMSUNG, {"0xE0E0F00F"});
  commands[SAMSUNG_CHLIST]            = makeCommandData(IR_SAMSUNG, {"0xE0E0D629"});
  commands[SAMSUNG_CHANNEL_UP]        = makeCommandData(IR_SAMSUNG, {"0xE0E048B7"});
  commands[SAMSUNG_CHANNEL_DOWN]      = makeCommandData(IR_SAMSUNG, {"0xE0E008F7"});
  commands[SAMSUNG_MENU]              = makeCommandData(IR_SAMSUNG, {"0xE0E058A7"});
  commands[SAMSUNG_APPS]              = makeCommandData(IR_SAMSUNG, {"0xE0E09E61"});
  commands[SAMSUNG_GUIDE]             = makeCommandData(IR_SAMSUNG, {"0xE0E0F20D"});
  commands[SAMSUNG_TOOLS]             = makeCommandData(IR_SAMSUNG, {"0xE0E0D22D"});
  commands[SAMSUNG_INFO]              = makeCommandData(IR_SAMSUNG, {"0xE0E0F807"});
  commands[SAMSUNG_UP]                = makeCommandData(IR_SAMSUNG, {"0xE0E006F9"});
  commands[SAMSUNG_DOWN]              = makeCommandData(IR_SAMSUNG, {"0xE0E08679"});
  commands[SAMSUNG_LEFT]              = makeCommandData(IR_SAMSUNG, {"0xE0E0A659"});
  commands[SAMSUNG_RIGHT]             = makeCommandData(IR_SAMSUNG, {"0xE0E046B9"});
  commands[SAMSUNG_SELECT]            = makeCommandData(IR_SAMSUNG, {"0xE0E016E9"});
  commands[SAMSUNG_RETURN]            = makeCommandData(IR_SAMSUNG, {"0xE0E01AE5"});
  commands[SAMSUNG_EXIT]              = makeCommandData(IR_SAMSUNG, {"0xE0E0B44B"});
  commands[SAMSUNG_KEY_A]             = makeCommandData(IR_SAMSUNG, {"0xE0E036C9"});
  commands[SAMSUNG_KEY_B]             = makeCommandData(IR_SAMSUNG, {"0xE0E028D7"});
  commands[SAMSUNG_KEY_C]             = makeCommandData(IR_SAMSUNG, {"0xE0E0A857"});
  commands[SAMSUNG_KEY_D]             = makeCommandData(IR_SAMSUNG, {"0xE0E06897"});
  commands[SAMSUNG_FAMILYSTORY]       = makeCommandData(IR_SAMSUNG, {"0xE0E0639C"});
  commands[SAMSUNG_SEARCH]            = makeCommandData(IR_SAMSUNG, {"0xE0E0CE31"});
  commands[SAMSUNG_DUALI_II]          = makeCommandData(IR_SAMSUNG, {"0xE0E000FF"});
  commands[SAMSUNG_SUPPORT]           = makeCommandData(IR_SAMSUNG, {"0xE0E0FC03"});
  commands[SAMSUNG_PSIZE]             = makeCommandData(IR_SAMSUNG, {"0xE0E07C83"});
  commands[SAMSUNG_ADSUBT]            = makeCommandData(IR_SAMSUNG, {"0xE0E0A45B"});
  commands[SAMSUNG_REWIND]            = makeCommandData(IR_SAMSUNG, {"0xE0E0A25D"});
  commands[SAMSUNG_PAUSE]             = makeCommandData(IR_SAMSUNG, {"0xE0E052AD"});
  commands[SAMSUNG_FASTFORWARD]       = makeCommandData(IR_SAMSUNG, {"0xE0E012ED"});
  commands[SAMSUNG_RECORD]            = makeCommandData(IR_SAMSUNG, {"0xE0E0926D"});
  commands[SAMSUNG_PLAY]              = makeCommandData(IR_SAMSUNG, {"0xE0E0E21D"});
  commands[SAMSUNG_STOP]              = makeCommandData(IR_SAMSUNG, {"0xE0E0629D"});
  commands[SAMSUNG_POWER_OFF]         = makeCommandData(IR_SAMSUNG, {"0xE0E019E6"});
  commands[SAMSUNG_POWER_ON]          = makeCommandData(IR_SAMSUNG, {"0xE0E09966"});
  commands[SAMSUNG_INPUT_HDMI_1]      = makeCommandData(IR_SAMSUNG, {"0xE0E09768"});
  commands[SAMSUNG_INPUT_HDMI_2]      = makeCommandData(IR_SAMSUNG, {"0xE0E07D82"});
  commands[SAMSUNG_INPUT_HDMI_3]      = makeCommandData(IR_SAMSUNG, {"0xE0E043BC"});
  commands[SAMSUNG_INPUT_HDMI_4]      = makeCommandData(IR_SAMSUNG, {"0xE0E0A35C"});
  commands[SAMSUNG_INPUT_COMPONENT]   = makeCommandData(IR_SAMSUNG, {"0xE0E0619E"});
  commands[SAMSUNG_INPUT_TV]          = makeCommandData(IR_SAMSUNG, {"0xE0E0D827"});
  // unknown commands. Not on my remote
  // commands[-]                         = makeCommandData(IR_SAMSUNG, {"0xE0E0C43B"});
  // commands[favorite_channel]          = makeCommandData(IR_SAMSUNG, {"0xE0E022DD"});

  // GC also works well
  //commands[SAMSUNG_POWER_TOGGLE]      = makeCommandData(IR_GC, {"38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798"});
  //commands[SAMSUNG_POWER_OFF]         = makeCommandData(IR_GC, {"38000,1,1,173,173,21,65,21,65,21,65,21,21,21,21,21,21,21,21,21,21,21,65,21,65,21,65,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,65,21,65,21,21,21,21,21,65,21,65,21,65,21,65,21,21,21,21,21,65,21,65,21,21,21,1832"});
  //commands[SAMSUNG_POWER_ON]          = makeCommandData(IR_GC, {"38000,1,1,172,172,22,64,22,64,22,64,22,21,22,21,22,21,22,21,22,21,22,64,22,64,22,64,22,21,22,21,22,21,22,21,22,21,22,64,22,21,22,21,22,64,22,64,22,21,22,21,22,64,22,21,22,64,22,64,22,21,22,21,22,64,22,64,22,21,22,1820"});
  //commands[SAMSUNG_INPUT_HDMI_1]      = makeCommandData(IR_GC, {"38000,1,1,173,173,21,65,21,65,21,65,21,21,21,21,21,21,21,21,21,21,21,65,21,65,21,65,21,21,21,21,21,21,21,21,21,21,21,65,21,21,21,21,21,65,21,21,21,65,21,65,21,65,21,21,21,65,21,65,21,21,21,65,21,21,21,21,21,21,21,1832"});
  //commands[SAMSUNG_INPUT_HDMI_2]      = makeCommandData(IR_GC, {"38000,1,1,173,173,21,65,21,65,21,65,21,21,21,21,21,21,21,21,21,21,21,65,21,65,21,65,21,21,21,21,21,21,21,21,21,21,21,21,21,65,21,65,21,65,21,65,21,65,21,21,21,65,21,65,21,21,21,21,21,21,21,21,21,21,21,65,21,21,21,1832"});
  //commands[SAMSUNG_TV]                = makeCommandData(IR_GC, {"38000,1,1,172,172,21,64,21,64,21,64,21,21,21,21,21,21,21,21,21,21,21,64,21,64,21,64,21,21,21,21,21,21,21,21,21,21,21,64,21,64,21,21,21,64,21,64,21,21,21,21,21,21,21,21,21,21,21,64,21,21,21,21,21,64,21,64,21,64,21,1673"});
}
