
class BatteryInterface {
    public:
        virtual void setup(DisplayInterface& display, int adc_pin, int charging_pin) = 0;
        virtual int getPercentage() = 0;
        virtual bool isCharging() = 0;
        virtual bool isConnected() = 0;
        virtual void update() = 0;
};