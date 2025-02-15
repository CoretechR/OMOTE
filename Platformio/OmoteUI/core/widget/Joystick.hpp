#pragma once

// #if __has_include("lvgl_joystick.h")
#include <functional>

#include "WidgetBase.hpp"
#include "lvgl_joystick.h"

namespace UI::Widget {

class Joystick : public Base {
 public:
  using UpdateHandler = std::function<void(int16_t, int16_t)>;
  Joystick(Joystick::UpdateHandler, int aRadius, int aStickRadius,
           lv_align_t aAlign = LV_ALIGN_CENTER, lv_style_t* aStyle = nullptr,
           lv_style_t* aStickStyle = nullptr);

  ~Joystick() override;

 private:
  uint8_t mId = 0;

  static void AllJoysticksUpdateHandler(uint8_t joystick_id, int16_t x,
                                        int16_t y);

  static inline std::vector<UpdateHandler> mJoystickHandlers{};
};

}  // namespace UI::Widget

// #endif