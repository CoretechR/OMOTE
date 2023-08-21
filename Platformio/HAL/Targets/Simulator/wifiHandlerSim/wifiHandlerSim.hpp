#pragma once
#include "wifiHandlerInterface.h"
#include "Notification.hpp"
#include <memory>

class wifiHandlerSim: public wifiHandlerInterface {
    public:
        wifiHandlerSim();
        static std::shared_ptr<wifiHandlerSim> getInstance();

        
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
};