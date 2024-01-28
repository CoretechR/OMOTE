#include "commandHandler.h"
#include "device_yamahaAmp/device_yamahaAmp.h"

void register_device_yamaha() {
  commands[YAMAHA_INPUT_DVD]           = makeCommandData(IR_NEC, {"0x5EA1837C"});
  commands[YAMAHA_INPUT_DTV]           = makeCommandData(IR_NEC, {"0x5EA12AD5"});
  commands[YAMAHA_INPUT_VCR]           = makeCommandData(IR_NEC, {"0x5EA1F00F"});
  commands[YAMAHA_POWER_TOGGLE]        = makeCommandData(IR_NEC, {"0x5EA1F807"});
  commands[YAMAHA_INPUT_CD]            = makeCommandData(IR_NEC, {"0x5EA1A857"});
  commands[YAMAHA_INPUT_MD]            = makeCommandData(IR_NEC, {"0x5EA1936C"});
  commands[YAMAHA_INPUT_VAUX]          = makeCommandData(IR_NEC, {"0x5EA1AA55"});
  commands[YAMAHA_MULTICHANNEL]        = makeCommandData(IR_NEC, {"0x5EA1E11E"});
  commands[YAMAHA_INPUT_TUNER]         = makeCommandData(IR_NEC, {"0x5EA16897"});
  commands[YAMAHA_PRESETGROUP]         = makeCommandData(IR_NEC, {"0x5EA148B7"});
  commands[YAMAHA_PRESETSTATION_MINUS] = makeCommandData(IR_NEC, {"0x5EA18877"});
  commands[YAMAHA_PRESETSTATION_PLUS]  = makeCommandData(IR_NEC, {"0x5EA108F7"});
  commands[YAMAHA_STANDARD]            = makeCommandData(IR_NEC, {"0x5EA109F6"});
  commands[YAMAHA_5CHSTEREO]           = makeCommandData(IR_NEC, {"0x5EA1E916"});
  commands[YAMAHA_NIGHT]               = makeCommandData(IR_NEC, {"0x5EA1A956"});
  commands[YAMAHA_SLEEP]               = makeCommandData(IR_NEC, {"0x5EA1EA15"});
  commands[YAMAHA_TEST]                = makeCommandData(IR_NEC, {"0x5EA1A15E"});
  commands[YAMAHA_STRAIGHT]            = makeCommandData(IR_NEC, {"0x5EA16A95"});
  commands[YAMAHA_VOL_MINUS]           = makeCommandData(IR_NEC, {"0x5EA1D827"});
  commands[YAMAHA_VOL_PLUS]            = makeCommandData(IR_NEC, {"0x5EA158A7"});
  commands[YAMAHA_PROG_MINUS]          = makeCommandData(IR_NEC, {"0x5EA19A65"});
  commands[YAMAHA_PROG_PLUS]           = makeCommandData(IR_NEC, {"0x5EA11AE5"});
  commands[YAMAHA_MUTE_TOGGLE]         = makeCommandData(IR_NEC, {"0x5EA138C7"});
  commands[YAMAHA_LEVEL]               = makeCommandData(IR_NEC, {"0x5EA1619E"});
  commands[YAMAHA_SETMENU]             = makeCommandData(IR_NEC, {"0x5EA139C6"});
  commands[YAMAHA_SETMENU_UP]          = makeCommandData(IR_NEC, {"0x5EA119E6"});
  commands[YAMAHA_SETMENU_DOWN]        = makeCommandData(IR_NEC, {"0x5EA19966"});
  commands[YAMAHA_SETMENU_MINUS]       = makeCommandData(IR_NEC, {"0x5EA1CA35"});
  commands[YAMAHA_SETMENU_PLUS]        = makeCommandData(IR_NEC, {"0x5EA14AB5"});
  commands[YAMAHA_POWER_OFF]           = makeCommandData(IR_NEC, {"0x5EA17887"});
  commands[YAMAHA_POWER_ON]            = makeCommandData(IR_NEC, {"0x5EA1B847"});

  // GC seems not to work
  //commands[YAMAHA_POWER_TOGGLE]       = makeCommandData(IR_GC, {"38000,1,69,341,170,21,21,21,64,21,64,21,64,21,64,21,64,21,64,21,21,21,64,21,21,21,21,21,21,21,21,21,21,21,21,21,64,21,21,21,64,21,21,21,64,21,21,21,64,21,21,21,21,21,64,21,21,21,64,21,21,21,64,21,21,21,64,21,64,21,1517,341,85,21,3655"});
  //commands[YAMAHA_POWER_OFF]          = makeCommandData(IR_GC, {"38000,1,69,341,170,21,21,21,64,21,64,21,64,21,64,21,64,21,64,21,21,21,64,21,21,21,21,21,21,21,21,21,21,21,21,21,64,21,64,21,64,21,64,21,64,21,64,21,64,21,64,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,64,21,1517,341,85,21,3655"});
  //commands[YAMAHA_POWER_ON]           = makeCommandData(IR_GC, {"38000,1,69,341,170,21,21,21,64,21,64,21,64,21,64,21,64,21,64,21,21,21,64,21,21,21,21,21,21,21,21,21,21,21,21,21,64,21,21,21,64,21,64,21,64,21,64,21,64,21,64,21,21,21,64,21,21,21,21,21,21,21,21,21,21,21,21,21,64,21,1517,341,85,21,3655"});
  //commands[YAMAHA_INPUT_DVD]          = makeCommandData(IR_GC, {"38000,1,69,341,170,21,21,21,64,21,21,21,64,21,64,21,64,21,64,21,21,21,64,21,21,21,64,21,21,21,21,21,21,21,21,21,64,21,64,21,21,21,21,21,21,21,21,21,21,21,64,21,64,21,21,21,64,21,64,21,64,21,64,21,64,21,21,21,21,21,1517,341,85,21,3655"});
  //commands[YAMAHA_INPUT_DTV]          = makeCommandData(IR_GC, {"38000,1,69,341,170,21,21,21,64,21,21,21,64,21,64,21,64,21,64,21,21,21,64,21,21,21,64,21,21,21,21,21,21,21,21,21,64,21,21,21,21,21,64,21,21,21,64,21,21,21,64,21,21,21,64,21,64,21,21,21,64,21,21,21,64,21,21,21,64,21,1517,341,85,21,3655"});
  //commands[YAMAHA_STANDARD]           = makeCommandData(IR_GC, {"38000,1,69,341,170,21,21,21,64,21,64,21,64,21,64,21,64,21,64,21,21,21,64,21,21,21,21,21,21,21,21,21,21,21,21,21,64,21,21,21,64,21,64,21,64,21,64,21,64,21,64,21,64,21,64,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,1517,341,85,21,3655"});
}
