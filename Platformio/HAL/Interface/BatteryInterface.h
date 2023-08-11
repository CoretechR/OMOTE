#include "DisplayInterface.h"

class BatteryInterface {
    public:
        struct batteryStatus {
            /// @brief Percent of battery remaining (0-100]
            int percentage;
            /// @brief Voltage of battery in millivolts
            int voltage;
            /// @brief True - Battery is Charging
            ///        False - Battery discharging
            bool isCharging;
        };
 
        virtual void setup(DisplayInterface& display, int adc_pin, int charging_pin) = 0;
        virtual int getPercentage() = 0;
        virtual bool isCharging() = 0;
        virtual bool isConnected() = 0;
        virtual void update() = 0;
};