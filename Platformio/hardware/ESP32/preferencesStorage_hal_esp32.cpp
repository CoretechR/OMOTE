#include <Preferences.h>
#include "sleep_hal_esp32.h"
#include "tft_hal_esp32.h"

Preferences preferences;

std::string currentScene;
std::string currentGUIname;

void init_preferences_HAL(void) {
  // Restore settings from internal flash memory
  preferences.begin("settings", false);
  if (preferences.getBool("alreadySetUp")) {
    // from sleep.h
    set_wakeupByIMUEnabled_HAL(preferences.getBool("wkpByIMU"));
    set_sleepTimeout_HAL(preferences.getUInt("slpTimeout"));
    // from tft.h
    set_backlightBrightness_HAL(preferences.getUChar("blBrightness"));
    // from here
    currentScene = std::string(preferences.getString("currentScene").c_str());
    currentGUIname = std::string(preferences.getString("currentGUIname").c_str());

    // Serial.printf("Preferences restored: brightness %d, GUI %s, scene %s\r\n", get_backlightBrightness_HAL(), get_currentGUIname().c_str(), get_currentScene().c_str());
  } else {
    // Serial.printf("No preferences to restore\r\n");
  }
  preferences.end();
}

void save_preferences_HAL(void) {
  preferences.begin("settings", false);
  // from sleep.h
  preferences.putBool("wkpByIMU", get_wakeupByIMUEnabled_HAL());
  // from tft.h
  preferences.putUInt("slpTimeout", get_sleepTimeout_HAL());
  preferences.putUChar("blBrightness", get_backlightBrightness_HAL());
  // from here
  preferences.putString("currentScene", currentScene.c_str());
  preferences.putString("currentGUIname", currentGUIname.c_str());
  if (!preferences.getBool("alreadySetUp")) {
    preferences.putBool("alreadySetUp", true);
  }
  preferences.end();
}

std::string get_currentScene_HAL() {
  return currentScene;
}
void set_currentScene_HAL(std::string aCurrentScene) {
  currentScene = aCurrentScene;
}
std::string get_currentGUIname_HAL(){
  return currentGUIname;
}
void set_currentGUIname_HAL(std::string aCurrentGUIname) {
  currentGUIname = aCurrentGUIname;
}
