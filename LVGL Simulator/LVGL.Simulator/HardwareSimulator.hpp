#pragma once
#include "HardwareAbstractionInterface.h"
#include <string>
#include <iostream>

#define screenWidth 240
#define screenHeight 320

class HardwareSimulator :
    public HardwareAbstractionInterface
{
    public:

    HardwareSimulator() = default;

    virtual void debugPrint(std::string message) override {
        std::cout << message;
    }

    virtual void sendIR() override {
        
    }

    virtual void MQTTPublish(const char* topic, const char* payload) override {

    };

    virtual void initLVGL(
        display_flush_cb aDisplayFlushCb = nullptr,
        touch_pad_read aTouchPadReadCb = nullptr) override {
        lv_init();
    };

    virtual lv_coord_t getScreenWidth() { return screenWidth; };
    virtual lv_coord_t getScreenHeight() { return screenHeight; };

};

