#include "battery.hpp"
#include <Arduino.h>

Battery::Battery(int adc_pin, int charging_pin): BatteryInterface(),
    mAdcPin(adc_pin),
    mChargingPin(charging_pin)
{
    mAdcPin = adc_pin;
    mChargingPin = charging_pin;
    // Power Pin Definition
    pinMode(mChargingPin, INPUT_PULLUP);
    pinMode(mAdcPin, INPUT);
}

int Battery::getPercentage()
{
    return constrain(map(this->getVoltage(), 3700, 4200, 0, 100), 0, 100);
}

bool Battery::isCharging()
{
    return !digitalRead(mChargingPin);
}

bool Battery::isConnected()
{
    return ((!isCharging()) && (getVoltage() < 4350));
}

int Battery::getVoltage()
{
    return analogRead(mAdcPin)*2*3300/4095 + 350;
}