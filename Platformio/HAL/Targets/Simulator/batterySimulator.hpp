#include "BatteryInterface.h"
#include <chrono>
#include <thread>
#include <cmath>
class BatterySimulator: public BatteryInterface{
    public:
        BatterySimulator() : 
            mCreationTime(std::chrono::high_resolution_clock::now()),
            mBattNotifier(std::thread(&BatterySimulator::batteryNotifyThread,this))
        {};

        ~BatterySimulator(){
            mBattNotifier.join();
        }
        virtual int getPercentage() override { 
            auto now = std::chrono::high_resolution_clock::now();
            auto batteryRunTime = std::chrono::duration_cast<std::chrono::seconds>(now - mCreationTime);
            constexpr auto minToBatteryZero = 3;
            auto fakeBattPercentage = 100 - ((batteryRunTime / std::chrono::duration<float,std::ratio<60LL>>(minToBatteryZero)) * 100);
            return std::floor(fakeBattPercentage < 100 ? fakeBattPercentage : 0);
        }

        virtual bool isCharging() override { return true; }

    private:
        void batteryNotifyThread(){
            while (true){
                NotifyCurrentStatus();
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        }
        std::chrono::_V2::system_clock::time_point mCreationTime;
        std::thread mBattNotifier;
};