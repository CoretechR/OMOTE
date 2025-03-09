#pragma once
#include "ScreenBase.hpp"
namespace UI::Screen {
/// @brief Due to the way LVGL utilizes screens we basically need a canvas to
/// create new pages on
/// by adding things to this screen and then moving them to their own page.
class BackgroundScreen : Base {
public:
  static BackgroundScreen *GetInstance();
  static lv_obj_t *getLvInstance();

private:
  BackgroundScreen();
  static BackgroundScreen *mInstance;
};
} // namespace UI::Screen