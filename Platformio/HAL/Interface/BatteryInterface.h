#pragma once
#include "DisplayInterface.h"

class BatteryInterface {
    public:
        BatteryInterface() = default;
        virtual int getPercentage() = 0;
        virtual bool isCharging() = 0;
};