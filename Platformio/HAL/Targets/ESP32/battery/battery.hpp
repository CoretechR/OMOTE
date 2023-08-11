#pragma once
#include "BatteryInterface.h"
#include "DisplayInterface.h"

class Battery: public BatteryInterface {
    public:

        virtual BatteryInterface::batteryStatus getBatteryPercentage();


        void setup(int adc_pin, int charging_pin);
        /**
         * @brief Get the Percentage of the battery
         * 
         * @return int Percentage of the battery 
         */
        int getPercentage();

        /**
         * @brief Function to determine if the battery is charging or not
         * 
         * @return true   Battery is currently charging 
         * @return false  Battery is currently not charging
         */
        bool isCharging();

        /**
         * @brief Function to determine if the battery is connected 
         * 
         * @return true   Battery is connected 
         * @return false  Battery is not connected
         */
        bool isConnected();

        /**
         * @brief Function to update the battery status. This should be called on a regular basis 
         * 
         */
        void update();
        // TODO move to cpp file
        Battery(){};
    private:
        /**
         * @brief Function to get the current voltage of the battery 
         * 
         * @return int Voltage of the battery in mV 
         */
        int getVoltage();

        /**
         * @brief Variable to store which pin should be used for ADC
         * 
         */
        int adc_pin;

        /**
         * @brief Variable to store which pin is used to inidicate if the battery is currently charging or not
         * 
         */
        int charging_pin;

};