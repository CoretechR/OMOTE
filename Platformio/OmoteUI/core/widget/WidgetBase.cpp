#include "WidgetBase.hpp"

namespace UI::Widget{

Base::Base(lv_obj_t* aLvglSelf): 
   mLvglSelf(aLvglSelf)
{
    mLvglSelf->user_data = this;
}


}