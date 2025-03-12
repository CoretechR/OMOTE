#pragma once
#include "Colors.hpp"
#include <lvgl.h>

namespace UI {
class UIElement;

class TextStyle {
  friend UIElement;

public:
  TextStyle() = default;

  TextStyle &Opacity(lv_opa_t aOpacity) {
    opacity = aOpacity;
    return *this;
  }
  TextStyle &Color(lv_color_t aColor) {
    color = aColor;
    return *this;
  }
  TextStyle &Decor(lv_text_decor_t aDecor) {
    decor = aDecor;
    return *this;
  }
  TextStyle &Align(lv_text_align_t aAlignment) {
    align = aAlignment;
    return *this;
  }
  TextStyle &LetterSpacing(lv_coord_t aLetterSpacing) {
    letter_space = aLetterSpacing;
    return *this;
  }
  TextStyle &LineSpacing(lv_coord_t aLineSpacing) {
    line_space = aLineSpacing;
    return *this;
  }
  TextStyle &Font(const lv_font_t *aFont) {
    font = aFont;
    return *this;
  }

private:
  lv_opa_t opacity = 255;
  lv_color_t color = Color::WHITE;
  lv_text_decor_t decor = LV_TEXT_DECOR_NONE;
  lv_text_align_t align = LV_TEXT_ALIGN_CENTER;
  lv_coord_t letter_space = 0;
  lv_coord_t line_space = 0;
  const lv_font_t *font = lv_font_default();
};
} // namespace UI