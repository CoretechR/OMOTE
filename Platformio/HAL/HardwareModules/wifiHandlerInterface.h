#pragma once
#include <string>

class wifiHandlerInterface{
    public:
        virtual void begin() = 0;
        virtual void connect(const char* SSID, const char* password) = 0;
        virtual void disconnect() = 0;
        virtual bool isConnected() = 0;
        virtual void turnOff() = 0;
        virtual void scan() = 0;
        virtual char* getSSID() = 0;
        virtual std::string getIP() = 0;
};