#include "battery.hpp"
#include "display.hpp"
#include <Arduino.h>

extern Display display;

Battery::Battery(int adc_pin, int charging_pin)
{
    this->adc_pin = adc_pin;
    this->charging_pin = charging_pin;
}

void Battery::setup()
{
  // Power Pin Definition
  pinMode(this->charging_pin, INPUT_PULLUP);
  pinMode(this->adc_pin, INPUT);
}

int Battery::getPercentage()
{
    return constrain(map(this->getVoltage(), 3700, 4200, 0, 100), 0, 100);
}

bool Battery::isCharging()
{
    return !digitalRead(this->charging_pin);
}

bool Battery::isConnected()
{
    return ((!this->isCharging()) && (this->getVoltage() < 4350));
}

int Battery::getVoltage()
{
    return analogRead(this->adc_pin)*2*3300/4095 + 350;
}

void Battery::update()
{
    display.update_battery(this->getPercentage(), this->isCharging(), this->isConnected());

}