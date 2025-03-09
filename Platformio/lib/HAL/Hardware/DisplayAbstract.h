#pragma once
#include <memory>

#include "lvgl.h"
class DisplayAbstract {
 public:
  virtual ~DisplayAbstract() = default;

  virtual void setBrightness(uint8_t brightness) = 0;
  virtual uint8_t getBrightness() = 0;
  virtual void turnOff() = 0;

  void ForceRefresh();

 protected:
  DisplayAbstract() = default;
  // Set this in the constructor of the Child Calss
  lv_display_t* mDisplay{nullptr};
  // Set this with a getInstance method in the Child Class
  static inline std::shared_ptr<DisplayAbstract> mInstance;
};

inline void DisplayAbstract::ForceRefresh() { lv_refr_now(mDisplay); }