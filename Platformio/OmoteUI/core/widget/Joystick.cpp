#include "Joystick.hpp"

// #if __has_include("lvgl_joystick.h")

#include "BackgroundScreen.hpp"
#include "WidgetBase.hpp"
#include "lvgl_joystick.h"
namespace UI::Widget {

// Currently this is implemented by statically holding onto the
// functions and then using ids to index into that structure

// A good update might be to do a lookup into the function array
// and then we can do actual clean up instead of just nulling the
// vectory entry

Joystick::Joystick(Joystick::UpdateHandler aJoystickHandler, int aRadius,
                   int aStickRadius, lv_align_t aAlign, lv_style_t* aStyle,
                   lv_style_t* aStickStyle)
    : Base(lv_obj_create(UI::Screen::BackgroundScreen::getLvInstance()),
           ID::Widgets::Joystick),
      mId(mJoystickHandlers.size()) {
  create_joystick(LvglSelf(), mId, aAlign, 0, 0, aRadius, aStickRadius, aStyle,
                  aStickStyle, Joystick::AllJoysticksUpdateHandler,
                  JOYSTICK_REPORT_MODE_ABSOLUTE);
  SetWidth(aRadius * 2 + 30);
  SetHeight(aRadius * 2 + 30);

  mJoystickHandlers.push_back(aJoystickHandler);
}

Joystick::~Joystick() { mJoystickHandlers[mId] = nullptr; }

void Joystick::AllJoysticksUpdateHandler(uint8_t joystick_id, int16_t x,
                                         int16_t y) {
  if (mJoystickHandlers.size() > joystick_id &&
      mJoystickHandlers[joystick_id]) {
    mJoystickHandlers[joystick_id](x, y);
  }
}

}  // namespace UI::Widget

// #endif