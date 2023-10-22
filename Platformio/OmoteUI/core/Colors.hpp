#pragma once
#include <lvgl.h>

namespace UI::Color {
const auto WHITE = lv_color_white();
const auto BLACK = lv_color_black();
const auto RED = lv_color_make(255, 0, 0);
const auto GREEN = lv_color_make(0, 255, 0);
const auto BLUE = lv_color_make(0, 0, 255);

const auto PURPLE = lv_color_make(128, 0, 128);
const auto LILAC = lv_color_make(231, 209, 255);

const auto GREY = lv_color_make(105, 105, 105);

} // namespace UI::Color