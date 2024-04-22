#include <string>
#include <lvgl.h>
#include "applicationInternal/gui/guiBase.h"
#include "applicationInternal/gui/guiMemoryOptimizer.h"
#include "applicationInternal/scenes/sceneRegistry.h"
#include "applicationInternal/hardware/hardwarePresenter.h"
#include "applicationInternal/commandHandler.h"
#include "scenes/scene__default.h"

void setLabelActiveScene() {
  if ((SceneLabel != NULL) && sceneExists(gui_memoryOptimizer_getActiveSceneName())) {
    lv_label_set_text(SceneLabel, gui_memoryOptimizer_getActiveSceneName().c_str());
  }
}

void navigateBackToSceneGUIlist();

void handleScene(uint16_t command, commandData commandData, std::string additionalPayload = "") {

  auto current = commandData.commandPayloads.begin();
  std::string scene_name = *current;

  // --- do not switch scene, but show scene selection gui. From that on, we are in the main_gui_list. ----------------
  if (scene_name == scene_name_selection) {
    Serial.println("scene: will show scene selection gui");
    guis_doTabCreationAfterGUIlistChanged(MAIN_GUI_LIST);
    return;
  }

  // --- do not switch scene, but navigate to the prev or next gui in the currently active list of guis ---------------
  if ((scene_name == scene_gui_next) || (scene_name == scene_gui_prev)) {
    if (scene_name == scene_gui_prev) {
      if (gui_memoryOptimizer_getActiveTabID() == 0) {
        Serial.println("scene: cannot navigate to prev gui, because there is none");
      } else {
        Serial.println("scene: will navigate to prev gui");
        setActiveTab(gui_memoryOptimizer_getActiveTabID() -1, LV_ANIM_ON, true);
      }

    } else if (scene_name == scene_gui_next) {
      if (!gui_memoryOptimizer_isTabIDInMemory(gui_memoryOptimizer_getActiveTabID() +1)) {
        Serial.println("scene: cannot navigate to next gui, because there is none");
      } else {
        Serial.println("scene: will navigate to next gui");
        setActiveTab(gui_memoryOptimizer_getActiveTabID() +1, LV_ANIM_ON, true);
      }

    }
    return;
  }

  // --- do not switch scene. Switch to the other gui list ------------------------------------------------------------
  if (scene_name == scene_back_to_previous_gui_list) {
    
    if (get_scene_has_gui_list(gui_memoryOptimizer_getActiveSceneName())) {
      Serial.println("scene: will navigate back to last active gui from previous gui list");
      guis_doTabCreationForNavigateToLastActiveGUIofPreviousGUIlist();
    } else {
      Serial.println("scene: cannot navigate back to last active gui from previous gui list, because no scene specific gui list was defined");
    }

    return;
  }

  // --- This is the normal case. We navigate to a new scene ----------------------------------------------------------

  // FORCE can be either as second payload in commandData
  // e.g. register_command(&SCENE_TV_FORCE  , makeCommandData(SCENE, {scene_name_TV, "FORCE"}));
  // or as additionalPayload, used by gui_sceneSelection.cpp
  // e.g. executeCommand(activate_scene_command, "FORCE");

  // we can have a second payload
  std::string isForcePayload = "";
  ++current;
  if (current != commandData.commandPayloads.end()) {
    isForcePayload = *current;
  }

  // check if we know the new scene
  if (!sceneExists(scene_name)) {
    Serial.printf("scene: cannot start scene %s, because it is unknown\r\n", scene_name.c_str());
    return;
  } else {
    Serial.printf("scene: will switch from old scene %s to new scene %s\r\n", gui_memoryOptimizer_getActiveSceneName().c_str(), scene_name.c_str());
  }

  // do not activate the same scene again, only when forced to do so (e.g. by long press on the gui or when selected by hardware key)
  bool callEndAndStartSequences;
  if ((scene_name == gui_memoryOptimizer_getActiveSceneName()) && ((isForcePayload != "FORCE") && (additionalPayload != "FORCE"))) {
    Serial.printf("scene: will not start scene again, because it is already active\r\n");
    callEndAndStartSequences = false;
  } else if ((scene_name == gui_memoryOptimizer_getActiveSceneName()) && ((isForcePayload == "FORCE") || (additionalPayload == "FORCE"))) {
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
    if (!sceneExists(gui_memoryOptimizer_getActiveSceneName()) && (gui_memoryOptimizer_getActiveSceneName() != "")) {
      Serial.printf("scene: WARNING: cannot end scene %s, because it is unknown\r\n", gui_memoryOptimizer_getActiveSceneName().c_str());
  
    } else {
      if (gui_memoryOptimizer_getActiveSceneName() != "") {
        Serial.printf("scene: will call end sequence for scene %s\r\n", gui_memoryOptimizer_getActiveSceneName().c_str());
        scene_end_sequence_from_registry(gui_memoryOptimizer_getActiveSceneName());
      }
  
    }

    // start new scene
    Serial.printf("scene: will call start sequence for scene %s\r\n", scene_name.c_str());
    scene_start_sequence_from_registry(scene_name);
  }

  gui_memoryOptimizer_setActiveSceneName(scene_name);

  if (SceneLabel != NULL) {lv_label_set_text(SceneLabel, gui_memoryOptimizer_getActiveSceneName().c_str());}

  Serial.printf("scene: scene handling finished, new scene %s is active\r\n", gui_memoryOptimizer_getActiveSceneName().c_str());

  guis_doTabCreationAfterGUIlistChanged(SCENE_GUI_LIST);
}

void handleGUI(uint16_t command, commandData commandData, std::string additionalPayload = "") {

  auto current = commandData.commandPayloads.begin();
  std::string GUIname = *current;

  // 1. check if the gui is known in the main_gui_list
  int gui_list_index = -1;
  // find index of gui_memoryOptimizer_getActiveGUIname() in gui_list_active
  for (int i=0; i < main_gui_list.size(); i++) {
    if (main_gui_list.at(i) == GUIname) {
      Serial.printf("handleGUI: found GUI with name \"%s\" in \"main_gui_list\" at position %d\r\n", GUIname.c_str(), i);
      gui_list_index = i;
      break;
    }
  }
  
  // 2. call guiBase.cpp
  if ((gui_list_index >= 0) && (gui_list_index < main_gui_list.size())) {
    guis_doTabCreationForSpecificGUI(MAIN_GUI_LIST, gui_list_index);

  } else {
    // gui was not found
    Serial.printf("handleGUI: GUI with name \"%s\" was not found. Cannot navigate to that GUI\r\n", GUIname.c_str());
    return;
  }  

}
