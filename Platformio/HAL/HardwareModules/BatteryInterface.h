#pragma once
#include "Notification.hpp"

class BatteryInterface {
public:
    BatteryInterface() = default;
    virtual int getPercentage() = 0;
    virtual bool isCharging() = 0;
};