#include "HomeScreen.hpp"

using namespace UI::Screen;

HomeScreen::HomeScreen() : Base(UI::ID::Screens::Home) {
  SetBgColor(lv_color_white());
  SetPushAnimation(LV_SCR_LOAD_ANIM_FADE_IN);
}