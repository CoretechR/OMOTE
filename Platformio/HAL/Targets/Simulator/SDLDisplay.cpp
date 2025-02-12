#include "SDLDisplay.hpp"

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

void SDLDisplay::setTitle(std::string aNewTitle) {
  SDL_SetWindowTitle(mSimWindow, aNewTitle.c_str());
}

SDLDisplay::SDLDisplay() : DisplayAbstract() {
  lv_group_set_default(lv_group_create());
  auto display = lv_sdl_window_create(SDL_HOR_RES, SDL_VER_RES);
  lv_display_set_default(display);

  lv_indev_t* mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, display);

  // Get the SDL window via an event
  SDL_Event aWindowIdFinder;
  SDL_PollEvent(&aWindowIdFinder);
  mSimWindow = SDL_GetWindowFromID(aWindowIdFinder.window.windowID);
}