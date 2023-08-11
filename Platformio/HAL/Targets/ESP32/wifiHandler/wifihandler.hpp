#pragma once
#include "wifiHandlerInterface.h"
#include <WiFi.h>

#define STRING_SIZE 50

class wifiHandler: public wifiHandlerInterface {
    public:
        static wifiHandler* getInstance();
        /**
         * @brief Function to initialize the wifi handler 
         * 
         */
        void begin();

        /**
         * @brief Connect to the wifi using the provided credetials 
         * 
         * @param SSID 
         * @param password 
         */
        void connect(const char* SSID, const char* password);

        /**
         * @brief Function to disconnect from the network 
         * 
         */
        void disconnect();

        /**
         * @brief Get the SSID of the found wifi
         * 
         * @param index     index of the found wifi 
         * @return String   SSID of the wifi
         */
        String getFoundSSID(unsigned int index);

        /**
         * @brief Get the RSSI of the found wifi
         * 
         * @param index     index of the found wifi
         * @return int      RSSI value of the found wifi 
         */
        int getFoundRSSI(unsigned int index);

        /**
         * @brief Function to determine wether or not we are connected to a network 
         * 
         * @return true  Device is connected to wifi network
         * @return false Device is not connected to wifi network
         */
        bool isConnected();

        /**
         * @brief Function to turn off wifi
         * 
         */
        void turnOff();

        /**
         * @brief function to trigger asynchronous scan for wifi networks
         * 
         */
        void scan();

        /**
         * @brief Function to get SSID of the currently connected wifi network
         * 
         * @return char* SSID of the currently connected network
         */
        char* getSSID();

        /**
         * @brief Function to update the wifi credentials. This function is called in the wifi event callback function
         * after a connection is established. Only then is the new credentials stored and the old stored credentials 
         * overwritten.
         * 
         * @param temporary_ssid 
         * @param temporary_password 
         */
        static void update_credentials(const char* temporary_ssid, const char* temporary_password);

        void WiFiEvent(WiFiEvent_t event);

        /**
         * @brief Function to get the IP address of this device 
         * 
         * @return String IP Address of the device
         */
        String getIP();
    private:

        static wifiHandler* instance;

        static char temporary_password[STRING_SIZE];
        static char temporary_ssid[STRING_SIZE];


        wifiHandler();

        /**
         * @brief Internal variable to store the wifi password 
         * 
         */
        static char password[STRING_SIZE];

        /**
         * @brief Internal variable to store the wifi SSID 
         * 
         */
        static char SSID[STRING_SIZE];

};