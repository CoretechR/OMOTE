#include "ScreenBase.hpp"

using namespace UI::Screen;

Base::Base(uint16_t aId) : UIElement(mScreen, aId), 
    mScreen(lv_obj_create(NULL))
{
}

void Base::AddWidget(Widget::Base::Ptr aWidget)
{
    mWidgets.push_back(std::move(aWidget));
}
