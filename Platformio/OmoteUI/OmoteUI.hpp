// OMOTE UI
// 2023 Matthew Colvin
#pragma once

#include "HardwareAbstract.hpp"
#include "Images.hpp"
#include "lvgl.h"
#include <algorithm>
#include <memory>
#include <stdio.h>
#include <string>


/// @brief Singleton to allow UI code to live separately from the Initialization
/// of resources.
class OmoteUI {
public:
  OmoteUI(std::shared_ptr<HardwareAbstract> aHardware)
      : mHardware(aHardware){};

  static std::weak_ptr<OmoteUI> getRefrence() { return getInstance(); };
  static std::shared_ptr<OmoteUI>
  getInstance(std::shared_ptr<HardwareAbstract> aHardware = nullptr) {
    if (mInstance) {
      return mInstance;
    } else if (aHardware) {
      mInstance = std::make_shared<OmoteUI>(aHardware);
    }
    return mInstance;
  };

  // Set the page indicator scroll position relative to the tabview scroll
  // position
  void store_scroll_value_event_cb(lv_event_t *e);
  // Update current device when the tabview page is changes
  void tabview_device_event_cb(lv_event_t *e);
  // Slider Event handler
  void bl_slider_event_cb(lv_event_t *e);
  // Apple Key Event handler
  void appleKey_event_cb(lv_event_t *e);
  // Wakeup by IMU Switch Event handler
  void WakeEnableSetting_event_cb(lv_event_t *e);
  // Smart Home Toggle Event handler
  void smartHomeToggle_event_cb(lv_event_t *e);
  // Smart Home Toggle Event handler
  void smartHomeSlider_event_cb(lv_event_t *e);
  // Virtual Keypad Event handler
  void virtualKeypad_event_cb(lv_event_t *e);
  void wifi_settings_cb(lv_event_t* event);

  void connect_btn_cb(lv_event_t* event);

  void password_field_event_cb(lv_event_t* e);
  // Use LVGL to layout the ui and register the callbacks
  void layout_UI();

  void ta_kb_event_cb(lv_event_t* e);

  void wifi_scan_done(std::shared_ptr<std::vector<WifiInfo>> info);
  void loopHandler();
  /**
   * @brief Function to hide the keyboard. If the keyboard is attached to a text area, it will be hidden when the
   * text area is defocused. This function can be used if the keyboard need to be hidden due to some script event. 
   * 
   */
  void hide_keyboard();

  /**
   * @brief Function to show the keyboard. If a text area needs the keybaord, it should be attached to the text area
   * using the approbiate function. The keyboard will then show up when the text area is focused. This function is
   * needed if the keyboard should be shown due to some script or other trigger.
   * 
   */
  void show_keyboard();

private:
  static std::shared_ptr<OmoteUI> mInstance;
  std::shared_ptr<HardwareAbstract> mHardware;
  void reset_settings_menu();
  void attach_keyboard(lv_obj_t* textarea);
  std::shared_ptr<std::vector<WifiInfo>> found_wifi_networks;
 /**
 * @brief Keyboard object used whenever a keyboard is needed.
 * 
 */
lv_obj_t* kb;

/**
 * @brief Function to create the keyboard object which can then be attached to different text areas.
 * 
 */
void create_keyboard();

  /**
   * @brief Set the up settings object
   * 
   * @param parent 
   */
  void setup_settings(lv_obj_t* parent);

 /**
   * @brief LVGL Menu for settings pages as needed.
   * 
   */
  lv_obj_t* settingsMenu;

  /**
   * @brief Main page of the settings menu 
   * 
   */
  lv_obj_t* settingsMainPage;

  /**
   * @brief Battery percentage label 
   * 
   */
  lv_obj_t* objBattPercentage;

  /**
   * @brief Battery icon object in the status bar 
   * 
   */
  lv_obj_t* objBattIcon;

  void create_status_bar();

  lv_obj_t *panel = nullptr;
  Images imgs = Images();
  uint_fast8_t currentDevice = 4;
  lv_color_t color_primary = lv_color_hex(0x303030); // gray
  bool wakeupByIMUEnabled = true;

  inline static const uint_fast8_t virtualKeyMapTechnisat[10] = {
      0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0};

/************************************** WIFI Settings Menu *******************************************************/
  /**
   * @brief Container within the wifi selection page
   */
  lv_obj_t* wifi_setting_cont;

  /**
   * @brief Wifi settings entry point on the settings tab
   * 
   */
  lv_obj_t* wifiOverview;

  /**
   * @brief Label in the wifi password page. This label is updated with the selected SSID when the credentials for
   * a wifi network is entered.
   * 
   */
  lv_obj_t* wifi_password_label;

  /**
   * @brief Menu Subpage for the wifi password
   */
  lv_obj_t* wifi_password_page;

  /**
   * @brief  Menu Subpage for wifi selection
   */
  lv_obj_t* wifi_selection_page;

  /**
   * @brief Wifi Label shown in the top status bar
   */
  lv_obj_t* WifiLabel;

  /**
   * @brief Number of wifi subpage needed to display the found wifi networks
   * 
   */
  unsigned int no_subpages;

  /**
   * @brief number of wifi networks found
   * 
   */
  unsigned int no_wifi_networks;


  void wifi_status(std::shared_ptr<wifiStatus> status);
  /**
   * @brief callback function to get next wifi subpage. This callback can be used to get the next or previous page
   * 
   * @param e lvgl event object 
   */
  void next_wifi_selection_subpage(lv_event_t* e);

  /**
   * @brief Create a wifi selection sub page object
   * 
   * @param menu         LVGL Menu where the sub page should be added to 
   * @return lv_obj_t*   Menu sub page object pointer
   */
  lv_obj_t* create_wifi_selection_page(lv_obj_t* menu);

  /**
   * @brief Method to create the wifi password sub page
   * 
   * @param menu        Menu where the sub page should be created 
   * @return lv_obj_t*  menu sub page object pointer
   */
  lv_obj_t* create_wifi_password_page(lv_obj_t* menu);

  /**
   * @brief Method to create the wifi settings on the main page
   * 
   * @param parent    lv object parent where the main settings page should be added to 
   */
  void create_wifi_main_page(lv_obj_t* parent);

  /**
   * @brief Method to create wifi settings. This method will call the create_wifi_selection_page, 
   * the create_wifi_password_page, and the create_wifi_main_page 
   * 
   * @param menu      Settings menu where the sub pages should be added to 
   * @param parent    lv object parent where the main settings page should be added to
   */
  void create_wifi_settings(lv_obj_t* menu, lv_obj_t* parent);
  
  /**
   * @brief Function to update the wifi selection sub page
   * 
   * @param page index of the page to display 
   */
  void update_wifi_selection_subpage(int page);

/************************************** Display settings menu ********************************************************/
  /**
   * Variable to store the current backlight brightness level
  */
  unsigned int backlight_brightness;

  /**
   * @brief Function to create the display settings page.
   * 
   * @param parent LVGL object acting as a parent for the display settings page
   */
    void display_settings(lv_obj_t* parent);
};
