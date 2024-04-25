// OMOTE firmware for ESP32
// 2023-2024 Maximilian Kern / Klaus Musch

// init hardware and hardware loop
#include "applicationInternal/hardware/hardwarePresenter.h"
// register devices and their commands
//   special
#include "devices/misc/device_specialCommands.h"
#include "applicationInternal/commandHandler.h"
//   keyboards
#include "devices/keyboard/device_keyboard_mqtt/device_keyboard_mqtt.h"
#include "devices/keyboard/device_keyboard_ble/device_keyboard_ble.h"
//   TV
#include "devices/TV/device_samsungTV/device_samsungTV.h"
//#include "devices/TV/device_lgTV/device_lgTV.h"
//   AV receiver
#include "devices/AVreceiver/device_yamahaAmp/device_yamahaAmp.h"
//#include "devices/AVreceiver/device_denonAvr/device_denonAvr.h"
//#include "devices/AVreceiver/device_lgsoundbar/device_lgsoundbar.h"
//   media player
#include "devices/mediaPlayer/device_appleTV/device_appleTV.h"
//#include "devices/mediaPlayer/device_lgbluray/device_lgbluray.h"
//#include "devices/mediaPlayer/device_samsungbluray/device_samsungbluray.h"
//#include "devices/mediaPlayer/device_shield/device_shield.h"
//   misc
#include "devices/misc/device_smarthome/device_smarthome.h"
// register gui and keys
#include "applicationInternal/gui/guiBase.h"
#include "applicationInternal/gui/guiRegistry.h"
#include "guis/gui_sceneSelection.h"
#include "guis/gui_irReceiver.h"
#include "guis/gui_settings.h"
#include "guis/gui_numpad.h"
#include "devices/AVreceiver/device_yamahaAmp/gui_yamahaAmp.h"
#include "devices/mediaPlayer/device_appleTV/gui_appleTV.h"
#include "devices/misc/device_smarthome/gui_smarthome.h"
#include "applicationInternal/keys.h"
#include "applicationInternal/gui/guiStatusUpdate.h"
// register scenes
#include "scenes/scene__default.h"
#include "scenes/scene_allOff.h"
#include "scenes/scene_TV.h"
#include "scenes/scene_fireTV.h"
#include "scenes/scene_chromecast.h"
#include "scenes/scene_appleTV.h"
#include "applicationInternal/scenes/sceneHandler.h"

#if defined(ARDUINO)
// in case of Arduino we have a setup() and a loop()
void setup() {

#elif defined(WIN32) || defined(__linux__)
// in case of Windows/Linux, we have only a main() function, no setup() and loop(), so we have to simulate them
// forward declaration of loop()
void loop(unsigned long *pIMUTaskTimer, unsigned long *pUpdateStatusTimer);
// main function as usual in C
int main(int argc, char *argv[]) {
#endif

  // --- Startup ---
  Serial.begin(115200);
  // do some general hardware setup, like powering the TFT, I2C, ...
  init_hardware_general();
  // get wakeup reason
  init_sleep();
  // Restore settings from internal flash memory
  init_preferences();
  // blinking led
  init_userled();
  // Power Pin definition
  init_battery();

  // button Pin definition for hardware keys
  init_keys();
  // setup IR sender
  init_infraredSender();
  #if (ENABLE_KEYBOARD_BLE == 1)
  init_keyboardBLE();
  #endif

  // register commands for the devices
  register_specialCommands();
  //   TV
  register_device_samsungTV();
//  register_device_lgTV();
  //   AV receiver
  register_device_yamahaAmp();
  //register_device_denonAvr();
  //register_device_lgsoundbar();
  //   media player
  register_device_appleTV();
  //register_device_lgbluray();
  //register_device_samsungbluray();
  //register_device_shield();
  //   misc
  register_device_smarthome();

  #if (ENABLE_KEYBOARD_MQTT == 1)
  register_device_keyboard_mqtt();
  #endif
  #if (ENABLE_KEYBOARD_BLE == 1)
  register_device_keyboard_ble();
  #endif
  register_keyboardCommands();

  // Register the GUIs. They will be displayed in the order they have been registered.
  register_gui_sceneSelection();
  register_gui_irReceiver();
  register_gui_settings();
  register_gui_appleTV();
  register_gui_numpad();
  register_gui_smarthome();
  register_gui_yamahaAmp();
  // Only show these GUIs in the main gui list. If you don't set this explicitely, by default all registered guis are shown.
  #if (USE_SCENE_SPECIFIC_GUI_LIST != 0)
  main_gui_list = {tabName_yamahaAmp, tabName_sceneSelection, tabName_smarthome, tabName_settings, tabName_irReceiver};
  #endif

  // register the scenes and their key_commands_*
  register_scene_defaultKeys();
  register_scene_TV();
  register_scene_fireTV();
  register_scene_chromecast();
  register_scene_appleTV();
  register_scene_allOff();
  // Only show these scenes on the sceneSelection gui. If you don't set this explicitely, by default all registered scenes are shown.
  set_scenes_on_sceneSelectionGUI({scene_name_TV, scene_name_fireTV, scene_name_chromecast, scene_name_appleTV});

  // init GUI - will initialize tft, touch and lvgl
  init_gui();
  setLabelActiveScene();
  gui_loop(); // Run the LVGL UI once before the loop takes over

  // setup the Inertial Measurement Unit (IMU) for motion detection. Has to be after init_gui(), otherwise I2C will not work
  init_IMU();

  // init WiFi - needs to be after init_gui() because WifiLabel must be available
  #if (ENABLE_WIFI_AND_MQTT == 1)
  init_mqtt();
  #endif

  Serial.printf("Setup finished in %lu ms.\r\n", millis());

  #if defined(WIN32) || defined(__linux__)
  // In Windows/Linux there is no loop function that is automatically being called. So we have to do this on our own infinitely here in main()
  unsigned long IMUTaskTimer = 0;
  unsigned long updateStatusTimer = 0;
  while (1)
    loop(&IMUTaskTimer, &updateStatusTimer);
  #endif

}

// Loop ------------------------------------------------------------------------------------------------------------------------------------
#if defined(ARDUINO)
unsigned long IMUTaskTimer = 0;
unsigned long updateStatusTimer = 0;
unsigned long *pIMUTaskTimer = &IMUTaskTimer;
unsigned long *pUpdateStatusTimer = &updateStatusTimer;
void loop() {
#elif defined(WIN32) || defined(__linux__)
void loop(unsigned long *pIMUTaskTimer, unsigned long *pUpdateStatusTimer) {
#endif

  // --- do as often as possible --------------------------------------------------------
  // update backlight brightness. Fade in on startup, dim before going to sleep
  update_backligthBrighness();
  // keypad handling: get key states from hardware and process them
  keypad_loop();
  // process IR receiver, if activated
  if (get_irReceiverEnabled()) {
    infraredReceiver_loop();
  }
  // update LVGL UI
  gui_loop();
  // call mqtt loop to receive mqtt messages, if you are subscribed to some topics
  #if (ENABLE_WIFI_AND_MQTT == 1)
  mqtt_loop();
  #endif

  // --- every 100 ms -------------------------------------------------------------------
  // Refresh IMU data (motion detection) every 100 ms
  // If no action (key, TFT or motion), then go to sleep
  if(millis() - *pIMUTaskTimer >= 100){
    *pIMUTaskTimer = millis();

    check_activity();

  }

  // --- every 1000 ms ------------------------------------------------------------------
  if(millis() - *pUpdateStatusTimer >= 1000) {
    *pUpdateStatusTimer = millis();

    // update user_led, battery, BLE, memoryUsage on GUI
    updateHardwareStatusAndShowOnGUI();
  }

}
