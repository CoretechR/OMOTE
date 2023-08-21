#include "HardwareSimulator.hpp"
#include <unistd.h>
#include "SDL2/SDL.h"
#include "display/monitor.h"
#include "indev/mouse.h"
#include "indev/mousewheel.h"
#include "indev/keyboard.h"
#include "sdl/sdl.h"

#include "SDLDisplay.hpp"


/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void * data)
{
    (void)data;

    while(1) {
        SDL_Delay(5);   /*Sleep for 5 millisecond*/
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

HardwareSimulator::HardwareSimulator(): HardwareAbstract(){
    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
     * Create an SDL thread to do this*/
    this->mDisplay = SDLDisplay::getInstance();
    this->mWifiHandler = std::make_shared<wifiHandlerSim>(wifiHandlerSim());
    SDL_CreateThread(tick_thread, "tick", NULL);
}

std::shared_ptr<wifiHandlerInterface> HardwareSimulator::wifi()
{
    return this->mWifiHandler;
}