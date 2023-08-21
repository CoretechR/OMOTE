#pragma once
#include "wifiHandlerInterface.h"
#include "Notification.hpp"
#include "memory.h"
#include <WiFi.h>

#define STRING_SIZE 50

class wifiHandler: public wifiHandlerInterface {
    public:
        wifiHandler();
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
        void connect(std::shared_ptr<std::string> ssid, std::shared_ptr<std::string> password);
        //void connect(const char* SSID, const char* password);



        /**
         * @brief function to trigger asynchronous scan for wifi networks
         * 
         */
        void scan();
        bool isAvailable();
        void onScanDone(std::function<void (std::shared_ptr<std::vector<WifiInfo>>)> function);
        void onStatusUpdate(std::function<void (std::shared_ptr<wifiStatus>)> function);


    private:

        Notification<std::shared_ptr<std::vector<WifiInfo>>> scan_notification;
        Notification<std::shared_ptr<wifiStatus>> status_update;
        /**
         * @brief Function to update the wifi credentials. This function is called in the wifi event callback function
         * after a connection is established. Only then is the new credentials stored and the old stored credentials 
         * overwritten.
         * 
         * @param temporary_ssid 
         * @param temporary_password 
         */
        void update_credentials();

        void WiFiEvent(WiFiEvent_t event);

        /**
         * @brief Function to turn off wifi
         * 
         */
        void turnOff();
        /**
         * @brief Function to get the IP address of this device 
         * 
         * @return String IP Address of the device
         */
        std::string getIP();
        wifiStatus wifi_status;
        static std::shared_ptr<wifiHandler> mInstance;
        std::shared_ptr<std::string> temporary_password;
        std::shared_ptr<std::string> temporary_ssid;

        void update_status();
        /**
         * @brief Internal variable to store the wifi password 
         * 
         */
        std::string password;

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
         * @brief Internal variable to store the wifi SSID 
         * 
         */
        std::string SSID;

};