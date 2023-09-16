#pragma once

#include "lvgl.h"

#include "UIElement.hpp"
#include "WidgetBase.hpp"

#include <vector>
namespace UI::Screen{

class Base : UIElement {
public:
    typedef std::unique_ptr<Base> Ptr;
    
    Base(uint16_t aId = 0);
    
    void AddWidget(Widget::Base::Ptr aWidget);

private:
    lv_obj_t *mScreen;
    std::vector<Widget::Base::Ptr> mWidgets;
};

}