#include <string>
#include <lvgl.h>
#include "applicationInternal/gui/guiBase.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "applicationInternal/commandHandler.h"
#include "scenes/scene__defaultKeys.h"

void handleScene(uint16_t command, commandData commandData, std::string additionalPayload = "") {

  // FORCE can be either as second payload in commandData
  // e.g. register_command(&SCENE_TV_FORCE  , makeCommandData(SCENE, {scene_name_TV, "FORCE"}));
  // or as additionalPayload, used by gui_sceneSelection.cpp
  // e.g. executeCommand(activate_scene_command, "FORCE");

  auto current = commandData.commandPayloads.begin();
  std::string scene_name = *current;
  // we can have a second payload
  std::string isForcePayload = "";
  ++current;
  if (current != commandData.commandPayloads.end()) {
    isForcePayload = *current;
  }

  // do not really switch scene, but show sceneSelection gui. From that on, we are in the main_gui_list.
  if (scene_name == scene_name_selection) {
    useSceneGUIlist = false;
    guis_doAfterSliding(-1, -1, true);
    return;
  }

  // check if we know the new scene
  if (!sceneExists(scene_name)) {
    Serial.printf("scene: cannot start scene %s, because it is unknown\r\n", scene_name.c_str());
    return;
  } else {
    Serial.printf("scene: will switch from old scene %s to new scene %s\r\n", get_currentScene().c_str(), scene_name.c_str());
  }

  // do not activate the same scene again, only when forced to do so (e.g. by long press on the gui or when selected by hardware key)
  bool callEndAndStartSequences;
  if ((scene_name == get_currentScene()) && ((isForcePayload != "FORCE") && (additionalPayload != "FORCE"))) {
    Serial.printf("scene: will not start scene again, because it is already active\r\n");
    callEndAndStartSequences = false;
  } else if ((scene_name == get_currentScene()) && ((isForcePayload == "FORCE") || (additionalPayload == "FORCE"))) {
    Serial.printf("scene: scene is already active, but FORCE was set, so start scene again\r\n");
    callEndAndStartSequences = true;
  } else {
    // this is the default when switching to a different scene
    callEndAndStartSequences = true;
  }

  if (SceneLabel != NULL) {lv_label_set_text(SceneLabel, "changing...");}
  gui_loop();

  if (callEndAndStartSequences) {
    // end old scene
    if (!sceneExists(get_currentScene()) && (get_currentScene() != "")) {
      Serial.printf("scene: WARNING: cannot end scene %s, because it is unknown\r\n", get_currentScene().c_str());
  
    } else {
      if (get_currentScene() != "") {
        Serial.printf("scene: will call end sequence for scene %s\r\n", get_currentScene().c_str());
        scene_end_sequence_from_registry(get_currentScene());
      }
  
    }

    // start new scene
    Serial.printf("scene: will call start sequence for scene %s\r\n", scene_name.c_str());
    scene_start_sequence_from_registry(scene_name);
  }

  set_currentScene(scene_name);

  if (SceneLabel != NULL) {lv_label_set_text(SceneLabel, get_currentScene().c_str());}

  Serial.printf("scene: scene handling finished, new scene %s is active\r\n", get_currentScene().c_str());

  useSceneGUIlist = true;
  // recreate the gui based on the current scene
  guis_doAfterSliding(-1, -1, true);
}

void setLabelCurrentScene() {
  if ((SceneLabel != NULL) && sceneExists(get_currentScene())) {
    lv_label_set_text(SceneLabel, get_currentScene().c_str());
  }
}
