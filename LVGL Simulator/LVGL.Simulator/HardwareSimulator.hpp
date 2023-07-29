#pragma once
#include "HardwareAbstractionInterface.h"
#include <string>
#include <iostream>

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

    virtual void init() override {
        lv_init();
    }

};

