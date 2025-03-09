#pragma once
#include <stdint.h>

#include "Hardware/DisplayAbstract.h"
#include "SDL2/SDL.h"

class SDLDisplay : public DisplayAbstract {
 public:
  static std::shared_ptr<SDLDisplay> getInstance();

  virtual void setBrightness(uint8_t brightness) override;
  virtual uint8_t getBrightness() override;
  virtual void turnOff() override;

  void setTitle(std::string aNewTitle);

 protected:
 private:
  SDLDisplay();
  uint8_t mBrightness;
  SDL_Window *mSimWindow;
};