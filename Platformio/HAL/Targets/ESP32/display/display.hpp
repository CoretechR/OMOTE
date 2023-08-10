#pragma once
#include "DisplayInterface.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Adafruit_FT6206.h>
#include "driver/ledc.h"

class Display:public DisplayInterface
{
    public:
        static Display* getInstance();
        /**
         * @brief  Function to setup the display.
         * 
         */
        void setup();

        /**
         * @brief Function to setup the user interface. This function has to be called after setup.
         * 
         */
        void setup_ui();

        /**
         * @brief Function to flush the display (update what is shown on the LCD). This function is called within
         * the LVGL callback function 
         * 
         * @param disp 
         * @param area 
         * @param color_p 
         */
        void flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );


        /**
         * @brief API function to inform display that wifi scan is completed 
         * 
         * @param size number of wifi networks found 
         */
        void wifi_scan_complete(unsigned int size);

        /**
         * @brief Clear the wifi networks listed in the wifi selection page. This function is called before new wifi
         * networks are added to the list to avoid double listing
         * 
         */
        void clear_wifi_networks();

        /**
         * @brief Update the wifi status. This function will update the wifi label in the status bar according to the
         * parameter. 
         * 
         * @param connected Boolean parameter to indicate if a wifi connection is established or not.
         */
        void update_wifi(bool connected);


        /**
         *  @brief API function which needs to be called regularly to update the display
         * 
         */
        void update();


        /**
         * @brief Function to update the battery indicator on the display 
         * 
         * @param percentage Battery percentage 
         * @param isCharging  True if the battery is charging. False otherwise
         * @param isConnected True if a battery is connected, false otherwise
         */
        void update_battery(int percentage, bool isCharging, bool isConnected);

        void turnOff();
    private:
        /**
         * @brief Function to change the settings menu to main page again.
         * 
         */
        void reset_settings_menu();
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
        Display();
        /**
         * @brief Pin used for LCD backlight control
         * 
         */
        int backlight_pin;

        /**
         * @brief Pin used to enable the LCD 
         * 
         */
        int enable_pin;

        /**
         * @brief Width of the display in pixel 
         * 
         */
        int width;

        /**
         * @brief Height of the display in pixel 
         * 
         */
        int height;

        /**
         * @brief Pointer to the buffer used for drawing on the screen 
         * 
         */
        lv_color_t *bufA;

        /**
         * @brief Pointer to the buffer used for drawing on the screen 
         * 
         */
        lv_color_t *bufB;
        
        /**
         * @brief Object of the touch input driver
         * 
         */
        Adafruit_FT6206 touch;
        /**
         * @brief Keyboard object used whenever a keyboard is needed.
         * 
         */
        lv_obj_t* kb;

        /**
         * @brief Variable to store the primary color
         * 
         */
        lv_color_t primary_color;

        /**
         * @brief Object of the TFT driver
         * 
         */
        TFT_eSPI tft;

        /**
         * @brief Set the up settings object
         * 
         * @param parent 
         */
        void setup_settings(lv_obj_t* parent);

        /**
         * @brief Function to create the keyboard object which can then be attached to different text areas.
         * 
         */
        void create_keyboard();

        /**
         * @brief Function to attach the keyboard to a text area. If the text area is selected, the keyboard
         * is shown and if the textarea is defocused, the keyboard will be hidden again.
         * 
         * @param textarea Textarea where the keyboard should be attached to. 
         */
        void attach_keyboard(lv_obj_t* textarea);

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