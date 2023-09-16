#pragma once

#include "lvgl.h"

namespace UI{

class UIElement{

public:
    UIElement(lv_obj_t* aLvglSelf, uint16_t aId = 0);

protected:
    lv_obj_t* mLvglSelf;
    uint16_t mId;
};
}