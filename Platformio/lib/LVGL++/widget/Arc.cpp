#include "Arc.hpp"

#include "BackgroundScreen.hpp"

using namespace UI::Widget;

Arc::Arc()
    : Base(lv_arc_create(UI::Screen::BackgroundScreen::getLvInstance()),
           ID::Widgets::Arc) {
  // Set default properties
  SetRange(0, 100);
  SetValue(0);
  SetRotation(0);
  SetMode(LV_ARC_MODE_NORMAL);
}

void Arc::SetValue(int16_t aValue) { lv_arc_set_value(LvglSelf(), aValue); }

void Arc::SetRange(int16_t aMin, int16_t aMax) {
  lv_arc_set_range(LvglSelf(), aMin, aMax);
}

void Arc::SetRotation(uint16_t aRotation) {
  lv_arc_set_rotation(LvglSelf(), aRotation);
}

void Arc::SetMode(lv_arc_mode_t aMode) { lv_arc_set_mode(LvglSelf(), aMode); }

int16_t Arc::GetValue() const { return lv_arc_get_value(LvglSelf()); }

int16_t Arc::GetMinValue() const { return lv_arc_get_min_value(LvglSelf()); }

int16_t Arc::GetMaxValue() const { return lv_arc_get_max_value(LvglSelf()); }

void Arc::OnLvglEvent(lv_event_t* anEvent) {
  // Handle any arc-specific events if needed
}
