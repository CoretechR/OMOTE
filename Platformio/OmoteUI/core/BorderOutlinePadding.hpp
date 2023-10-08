#pragma once
#include "Colors.hpp"
#include "UIElement.hpp"
#include <lvgl.h>
// Builder Class to make it easier to create boarder and Paddings
namespace UI {
/////////////////////////////////Border/////////////////////////////////
class Border {
  friend class UIElement;

public:
  Border() = default;

  Border &Width(lv_coord_t aWidth) {
    width = aWidth;
    return *this;
  }
  Border &Color(lv_color_t aColor) {
    color = aColor;
    return *this;
  }
  Border &Opacity(lv_opa_t aOpacity) {
    opacity = aOpacity;
    return *this;
  }
  Border &Sides(lv_border_side_t aSides) {
    sides = aSides;
    return *this;
  }

private:
  lv_coord_t width = 0;
  lv_color_t color = UI::Color::BLACK;
  lv_opa_t opacity = 255;
  lv_border_side_t sides = LV_BORDER_SIDE_FULL;
};

///////////////////////////////////////Outline////////////////////////////////
class Outline {
  friend class UIElement;

public:
  Outline() = default;

  Outline &Width(lv_coord_t aWidth) {
    width = aWidth;
    return *this;
  }
  Outline &Color(lv_color_t aColor) {
    color = aColor;
    return *this;
  }
  Outline &Opacity(lv_opa_t aOpacity) {
    opacity = aOpacity;
    return *this;
  }
  Outline &Padding(lv_coord_t aPadding) {
    padding = aPadding;
    return *this;
  }

private:
  lv_coord_t width = 0;
  lv_color_t color = UI::Color::BLACK;
  lv_opa_t opacity = 255;
  lv_coord_t padding = 0;
};

//////////////////////////////////////Padding////////////////////////////////
class Padding {
  friend class UIElement;

public:
  Padding() = default;

  Padding &Top(lv_coord_t aTopPadding) {
    top = aTopPadding;
    return *this;
  };
  Padding &Bottom(lv_coord_t aBottomPadding) {
    bottom = aBottomPadding;
    return *this;
  };
  Padding &Left(lv_coord_t aLeftPadding) {
    left = aLeftPadding;
    return *this;
  };
  Padding &Right(lv_coord_t aRightPadding) {
    right = aRightPadding;
    return *this;
  };
  Padding &Row(lv_coord_t aRowPadding) {
    row = aRowPadding;
    return *this;
  }
  Padding &Column(lv_coord_t aColumnPadding) {
    column = aColumnPadding;
    return *this;
  }

private:
  lv_coord_t top = 0;
  lv_coord_t bottom = 0;
  lv_coord_t left = 0;
  lv_coord_t right = 0;
  lv_coord_t row = 0;
  lv_coord_t column = 0;
};

} // namespace UI