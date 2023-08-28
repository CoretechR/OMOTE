#include "HardwareSimulator.hpp"
#include <unistd.h>
#include "SDL2/SDL.h"
#include "display/monitor.h"
#include "indev/mouse.h"
#include "indev/mousewheel.h"
#include "indev/keyboard.h"
#include "sdl/sdl.h"

#include "SDLDisplay.hpp"

HardwareSimulator::HardwareSimulator(): HardwareAbstract(), 
    mTickThread([](){
        while(true){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            lv_tick_inc(2); /*Tell lvgl that 2 milliseconds were elapsed*/
    }}),
    mBattery(std::make_shared<BatterySimulator>()),
    mDisplay(SDLDisplay::getInstance()),
    mWifiHandler(std::make_shared<wifiHandlerSim>())
{}

std::shared_ptr<BatteryInterface> HardwareSimulator::battery(){
    return mBattery;
}
std::shared_ptr<DisplayAbstract> HardwareSimulator::display(){
    return mDisplay;
}
std::shared_ptr<wifiHandlerInterface> HardwareSimulator::wifi(){
    return mWifiHandler;
}