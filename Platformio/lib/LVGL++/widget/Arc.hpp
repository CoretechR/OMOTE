#pragma once
#include "WidgetBase.hpp"

namespace UI::Widget {
class Arc : public Base {
public:
    Arc();
    virtual ~Arc() = default;

    void SetValue(int16_t aValue);
    void SetRange(int16_t aMin, int16_t aMax);
    void SetRotation(uint16_t aRotation);
    void SetMode(lv_arc_mode_t aMode);
    
    int16_t GetValue() const;
    int16_t GetMinValue() const;
    int16_t GetMaxValue() const;

protected:
    void OnLvglEvent(lv_event_t* anEvent) override;
};

} // namespace UI::Widget
