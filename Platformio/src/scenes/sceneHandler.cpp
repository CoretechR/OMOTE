#include <string>
#include "device_samsungTV/device_samsungTV.h"
#include "device_yamahaAmp/device_yamahaAmp.h"
#include "scenes/sceneRegistry.h"
#include "scenes/scene_allOff.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"
#include "scenes/scene_chromecast.h"
#include "commandHandler.h"
#include "gui_general_and_keys/guiBase.h"

std::string currentScene = "";

void handleScene(std::string command, commandData commandData, std::string additionalPayload = "") {

  auto current = commandData.commandPayloads.begin();
  std::string scene_name = *current;

  // check if we know the new scene
  if (!sceneExists(scene_name)) {
    Serial.printf("scene: cannot start scene %s, because it is unknown\r\n", scene_name.c_str());
    return;
  } else {
    Serial.printf("scene: will switch from old scene %s to new scene %s\r\n", currentScene.c_str(), scene_name.c_str());
  }

  if (SceneLabel != NULL) {lv_label_set_text(SceneLabel, "changing...");}
  gui_loop();

  // end old scene
  if (!sceneExists(currentScene) && (currentScene != "")) {
    Serial.printf("scene: WARNING: cannot end scene %s, because it is unknown\r\n", currentScene.c_str());

  } else {
    if (currentScene != "") {
      Serial.printf("scene: will call end sequence for scene %s\r\n", currentScene.c_str());
      scene_end_sequence_from_registry(currentScene);
    }

  }

  // start new scene
  Serial.printf("scene: will call start sequence for scene %s\r\n", scene_name.c_str());
  scene_start_sequence_from_registry(scene_name);

  currentScene = scene_name;

  if (SceneLabel != NULL) {lv_label_set_text(SceneLabel, currentScene.c_str());}

  Serial.printf("scene: scene handling finished, new scene %s is active\r\n", currentScene.c_str());
}

void setLabelCurrentScene() {
  if ((SceneLabel != NULL) && sceneExists(currentScene)) {
    lv_label_set_text(SceneLabel, currentScene.c_str());
  }
}
