#include "commandHandler.h"
#include "device_denonAvr/device_denonAvr.h"

void register_device_denon() {
	commands[DENON_POWER_TOGGLE] =      makeCommandData(IR_DENON, {"0x2A4C028A0088"});
	commands[DENON_POWER_SLEEP] =       makeCommandData(IR_DENON, {"0x2A4C02822CAC"});
	commands[DENON_VOL_MINUS] =         makeCommandData(IR_DENON, {"0x2A4C0288E862"});
	commands[DENON_VOL_PLUS] =          makeCommandData(IR_DENON, {"0x2A4C0280E86A"});
	commands[DENON_VOL_MUTE] =          makeCommandData(IR_DENON, {"0x2A4C0284E86E"});
	commands[DENON_CHAN_PLUS] =         makeCommandData(IR_DENON, {"0x2A4C0288DC56"});
	commands[DENON_CHAN_MINUS] =        makeCommandData(IR_DENON, {"0x2A4C0284DC5A"});
	commands[DENON_INPUT_CABLESAT] =    makeCommandData(IR_DENON, {"0x2A4C028CB43A"});
	commands[DENON_INPUT_MEDIAPLAYER] = makeCommandData(IR_DENON, {"0x2A4C0286B430"});
	commands[DENON_INPUT_BLURAY] =      makeCommandData(IR_DENON, {"0x2A4C0288B43E"});
	commands[DENON_INPUT_GAME] =        makeCommandData(IR_DENON, {"0x2A4C028AB43C"});
	commands[DENON_INPUT_AUX1] =        makeCommandData(IR_DENON, {"0x2A4C0289B43F"});
	commands[DENON_INPUT_AUX2] =        makeCommandData(IR_DENON, {"0x2A4C0285B433"});
	commands[DENON_INPUT_PHONO] =       makeCommandData(IR_DENON, {"0x2A4C028034B6"});
	commands[DENON_INPUT_TUNER] =       makeCommandData(IR_DENON, {"0x2A4C028F34B9"});
	commands[DENON_INPUT_TV] =          makeCommandData(IR_DENON, {"0x2A4C0284B432"});
	commands[DENON_INPUT_USB] =         makeCommandData(IR_DENON, {"0x2A4C028734B1"});
	commands[DENON_INPUT_BLUETOOTH] =   makeCommandData(IR_DENON, {"0x2A4C028F74F9"});
	commands[DENON_INPUT_INTERNET] =    makeCommandData(IR_DENON, {"0x2A4C028A74FC"});
	commands[DENON_INPUT_HEOS] =        makeCommandData(IR_DENON, {"0x2A4C028E34B8"});
	commands[DENON_POWER_ECO] =         makeCommandData(IR_DENON, {"0x2A4C02816CEF"});
	commands[DENON_INFO] =              makeCommandData(IR_DENON, {"0x2A4C0280E466"});
	commands[DENON_OPTION] =            makeCommandData(IR_DENON, {"0x2A4C028ADC54"});
	commands[DENON_BACK] =              makeCommandData(IR_DENON, {"0x2A4C028440C6"});
	commands[DENON_SETUP] =             makeCommandData(IR_DENON, {"0x2A4C028C40CE"});
	commands[DENON_MENU_ENTER] =        makeCommandData(IR_DENON, {"0x2A4C028F800D"});
	commands[DENON_MENU_UP] =           makeCommandData(IR_DENON, {"0x2A4C028D800F"});
	commands[DENON_MENU_LEFT] =         makeCommandData(IR_DENON, {"0x2A4C028B8009"});
	commands[DENON_MENU_RIGHT] =        makeCommandData(IR_DENON, {"0x2A4C02878005"});
	commands[DENON_MENU_DOWN] =         makeCommandData(IR_DENON, {"0x2A4C02838001"});
	commands[DENON_SOUNDMODE_MOVIE] =   makeCommandData(IR_DENON, {"0x2A4C028928A3"});
	commands[DENON_SOUNDMODE_MUSIC] =   makeCommandData(IR_DENON, {"0x2A4C028528AF"});
	commands[DENON_SOUNDMODE_GAME] =    makeCommandData(IR_DENON, {"0x2A4C028D28A7"});
	commands[DENON_SOUNDMODE_PURE] =    makeCommandData(IR_DENON, {"0x2A4C028AC840"});
	commands[DENON_QUICKSELECT_1] =     makeCommandData(IR_DENON, {"0x2A4C028248C8"});
	commands[DENON_QUICKSELECT_2] =     makeCommandData(IR_DENON, {"0x2A4C028A48C0"});
	commands[DENON_QUICKSELECT_3] =     makeCommandData(IR_DENON, {"0x2A4C028648CC"});
	commands[DENON_QUICKSELECT_4] =     makeCommandData(IR_DENON, {"0x2A4C028E48C4"});
	commands[DENON_MEDIA_PREV] =        makeCommandData(IR_DENON, {"0x2A4C028C7CF2"});
	commands[DENON_MEDIA_PLAYPAUSE] =   makeCommandData(IR_DENON, {"0x2A4C02807CFE"});
	commands[DENON_MEDIA_NEXT] =        makeCommandData(IR_DENON, {"0x2A4C02827CFC"});
}
