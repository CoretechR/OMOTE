#include "lvgl.h"
namespace UI::Widget{

class Base{
public:
    Base(lv_obj_t* aLvglSelf):
    mLvglSelf(aLvglSelf)
    {
        mLvglSelf.user_data = this;
    }
    
private:
    lv_obj_t* mLvglSelf;
    
    uint16_t mId;
};

}
