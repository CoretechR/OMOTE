#include "BatteryInterface.h"

class BatterySimulator: public BatteryInterface{
    public:
        BatterySimulator() {};
        virtual int getPercentage() override { return 75; }
        virtual bool isCharging() override { return true; }

};