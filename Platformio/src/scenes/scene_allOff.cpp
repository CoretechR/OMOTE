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

std::map<char, repeatModes> key_repeatModes_allOff {










};

std::map<char, std::string> key_commands_short_allOff {










};

std::map<char, std::string> key_commands_long_allOff {


};

void scene_start_sequence_allOff(void) {
  executeCommand(SAMSUNG_POWER_OFF);
  delay(500);
  executeCommand(YAMAHA_POWER_OFF);
  delay(500);
  // repeat IR to be sure
  executeCommand(SAMSUNG_POWER_OFF);
  delay(500);
  executeCommand(YAMAHA_POWER_OFF);
  delay(500);
  // repeat IR to be sure
  executeCommand(SAMSUNG_POWER_OFF);
  delay(500);
  executeCommand(YAMAHA_POWER_OFF);
  delay(500);
  // you cannot power off FireTV, but at least you can stop the currently running app
  executeCommand(KEYBOARD_HOME);
  delay(500);
  executeCommand(KEYBOARD_HOME);

}

void scene_end_sequence_allOff(void) {

}

std::string scene_name_allOff = "Off";

void register_scene_allOff(void){
  register_scene(
    scene_name_allOff,
    & scene_start_sequence_allOff,
    & scene_end_sequence_allOff,
    & key_repeatModes_allOff,
    & key_commands_short_allOff,
    & key_commands_long_allOff);

  commands[SCENE_ALLOFF]     = makeCommandData(SCENE, {scene_name_allOff});
}
