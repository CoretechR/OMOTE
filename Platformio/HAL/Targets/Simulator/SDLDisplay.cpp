#include "SDLDisplay.hpp"
#include "sdl/sdl.h"
#include <string>

std::shared_ptr<SDLDisplay> SDLDisplay::getInstance() {
  if (!DisplayAbstract::mInstance) {
    DisplayAbstract::mInstance = std::shared_ptr<SDLDisplay>(new SDLDisplay());
  }
  return std::static_pointer_cast<SDLDisplay>(mInstance);
}

void SDLDisplay::setBrightness(uint8_t brightness) { mBrightness = brightness; }

uint8_t SDLDisplay::getBrightness() { return mBrightness; }

void SDLDisplay::turnOff() {}

void SDLDisplay::flushDisplay(lv_disp_drv_t *disp, const lv_area_t *area,
                              lv_color_t *color_p) {
  sdl_display_flush(disp, area, color_p);
}

void SDLDisplay::screenInput(lv_indev_drv_t *indev_driver,
                             lv_indev_data_t *data) {
  sdl_mouse_read(indev_driver, data);
}

void SDLDisplay::setTitle(std::string aNewTitle) {
  SDL_SetWindowTitle(mSimWindow, aNewTitle.c_str());
}

SDLDisplay::SDLDisplay() : DisplayAbstract() {
  sdl_init();

  // Get the SDL window via an event
  SDL_Event aWindowIdFinder;
  SDL_PollEvent(&aWindowIdFinder);
  mSimWindow = SDL_GetWindowFromID(aWindowIdFinder.window.windowID);
}