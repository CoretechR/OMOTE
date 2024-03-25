#include <string>
#include <lvgl.h>
#include "applicationInternal/gui/guiBase.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "applicationInternal/commandHandler.h"

void handleScene(uint16_t command, commandData commandData, std::string additionalPayload = "") {

  auto current = commandData.commandPayloads.begin();
  std::string scene_name = *current;

  // check if we know the new scene
  if (!sceneExists(scene_name)) {
    Serial.printf("scene: cannot start scene %s, because it is unknown\r\n", scene_name.c_str());
    return;
  } else {
    Serial.printf("scene: will switch from old scene %s to new scene %s\r\n", get_currentScene().c_str(), scene_name.c_str());
  }

  if (SceneLabel != NULL) {lv_label_set_text(SceneLabel, "changing...");}
  gui_loop();

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

  set_currentScene(scene_name);

  if (SceneLabel != NULL) {lv_label_set_text(SceneLabel, get_currentScene().c_str());}

  Serial.printf("scene: scene handling finished, new scene %s is active\r\n", get_currentScene().c_str());
}

void setLabelCurrentScene() {
  if ((SceneLabel != NULL) && sceneExists(get_currentScene())) {
    lv_label_set_text(SceneLabel, get_currentScene().c_str());
  }
}
