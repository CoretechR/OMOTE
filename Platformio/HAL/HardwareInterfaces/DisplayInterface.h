#pragma once

class DisplayInterface
{
    public:
        virtual void setup() = 0;
        virtual void setup_ui() = 0;
        virtual void flush() = 0;
        virtual void wifi_scan_complete(unsigned int size) = 0;
        virtual void clear_wifi_networks() = 0;
        virtual void update_wifi(bool connected) = 0;
        virtual void hide_keyboard() = 0;
        virtual void show_keyboard() = 0;
        virtual void updated() = 0;
        virtual void reset_settings_menu() = 0;
        virtual void update_battery(int percentage, bool isCharging, bool isConnected) = 0;
        virtual void turnOff() = 0;
};