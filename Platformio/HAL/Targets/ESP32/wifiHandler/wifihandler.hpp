#pragma once
#include "wifiHandlerInterface.h"
#include <WiFi.h>

#define STRING_SIZE 50

class wifiHandler: public wifiHandlerInterface {
    public:
        static std::shared_ptr<wifiHandler> getInstance();
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
        void update_credentials(const char* temporary_ssid, const char* temporary_password);

        void WiFiEvent(WiFiEvent_t event);

        /**
         * @brief Function to get the IP address of this device 
         * 
         * @return String IP Address of the device
         */
        std::string getIP();
    private:

        wifiHandler();

        static std::shared_ptr<wifiHandler> mInstance;
        char temporary_password[STRING_SIZE];
        char temporary_ssid[STRING_SIZE];

        /**
         * @brief Internal variable to store the wifi password 
         * 
         */
        char password[STRING_SIZE];

        /**
         * @brief Internal variable to store the wifi SSID 
         * 
         */
        char SSID[STRING_SIZE];

};