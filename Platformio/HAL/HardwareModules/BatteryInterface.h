#pragma once
#include "Notification.hpp"

class BatteryInterface {
    public:
        BatteryInterface() = default;
        virtual int getPercentage() = 0;
        virtual bool isCharging() = 0;
        
        /// @brief Notify on Current battery status.
        void NotifyCurrentStatus();
        /// @brief Register Handler to be ran on battery status change
        /// @param Callable to be ran on status change (percentage, IsCharging)
        void onBatteryStatusChange(std::function<void(int,bool)>);
    private:
        Notification<int,bool> mBatteryNotification;
};