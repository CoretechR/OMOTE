#include "preferences_storage.h"
#include "sleep.h"
#include "tft.h"
#include "gui.h"
#include "commandHandler.h"

Preferences preferences;

void init_preferences(void) {
  // Restore settings from internal flash memory
  preferences.begin("settings", false);
  if(preferences.getBool("alreadySetUp")){
    wakeupByIMUEnabled = preferences.getBool("wkpByIMU");
    actualSleepTimeout = preferences.getUInt("slpTimeout");
    backlight_brightness = preferences.getUChar("blBrightness");
    currentScreen = preferences.getUChar("currentScreen");
    allDevsPowered = preferences.getUChar("allDevsPowered");
    currentScene = preferences.getString("currentScene");

    // Serial.printf("Preferences restored: brightness %d, screen %d, allDevPowered %d, scene %s\r\n", backlight_brightness, currentScreen, allDevsPowered, currentScene.c_str());
  } else {
    // Serial.printf("No preferences to restore\r\n");
  }
}
