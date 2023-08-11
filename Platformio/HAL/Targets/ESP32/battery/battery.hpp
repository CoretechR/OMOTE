#pragma once
#include "BatteryInterface.h"
#include "DisplayInterface.h"

class Battery: public BatteryInterface {
    public:

        /**
         * @brief Get the Percentage of the battery
         *
         * @return int Percentage of the battery
         */
        virtual int getPercentage() override;

        /**
         * @brief Function to determine if the battery is charging or not
         *
         * @return true   Battery is currently charging
         * @return false  Battery is currently not charging
         */
        virtual bool isCharging() override;

        /**
         * @brief Function to determine if the battery is connected
         *
         * @return true   Battery is connected
         * @return false  Battery is not connected
         */
        bool isConnected();

        Battery(int adc_pin, int charging_pin);

        // Not sure why this is needed but shared_ptr seems to really
        // need it possibly a compiler template handling limitation
        // none the less we really should not use it.
        Battery() = default;
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
        int mAdcPin;

        /**
         * @brief Variable to store which pin is used to indicate if the battery is currently charging or not
         *
         */
        int mChargingPin;

};