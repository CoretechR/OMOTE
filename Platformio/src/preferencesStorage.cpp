#include "preferencesStorage.h"
#include "hardware/sleep.h"
#include "hardware/tft.h"
#include "gui_general_and_keys/guiBase.h"
#include "commandHandler.h"
#include "scenes/sceneHandler.h"

Preferences preferences;

void init_preferences(void) {
  // Restore settings from internal flash memory
  preferences.begin("settings", false);
  if(preferences.getBool("alreadySetUp")){
    wakeupByIMUEnabled = preferences.getBool("wkpByIMU");
    actualSleepTimeout = preferences.getUInt("slpTimeout");
    backlight_brightness = preferences.getUChar("blBrightness");
    currentScreen = preferences.getUChar("currentScreen");
    currentScene = std::string(preferences.getString("currentScene").c_str());

    // Serial.printf("Preferences restored: brightness %d, screen %d, scene %s\r\n", backlight_brightness, currentScreen, currentScene.c_str());
  } else {
    // Serial.printf("No preferences to restore\r\n");
  }
  preferences.end();
}

void save_preferences(void) {
  preferences.begin("settings", false);
  preferences.putBool("wkpByIMU", wakeupByIMUEnabled);
  preferences.putUInt("slpTimeout", actualSleepTimeout);
  preferences.putUChar("blBrightness", backlight_brightness);
  preferences.putUChar("currentScreen", currentScreen);
  preferences.putString("currentScene", currentScene.c_str());
  if(!preferences.getBool("alreadySetUp")) preferences.putBool("alreadySetUp", true);
  preferences.end();
}

