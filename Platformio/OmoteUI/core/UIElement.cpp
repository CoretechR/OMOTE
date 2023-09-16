#include "UIElement.hpp"

namespace UI
{
    UIElement::UIElement(lv_obj_t* aLvglSelf, uint16_t aId):
    mLvglSelf(aLvglSelf),
    mId(aId)
    {
        mLvglSelf->user_data = this;
    }

} // namespace UI
