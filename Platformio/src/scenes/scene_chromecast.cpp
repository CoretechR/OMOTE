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










};

std::map<char, std::string> key_commands_short_chromecast {










};

std::map<char, std::string> key_commands_long_chromecast {


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
