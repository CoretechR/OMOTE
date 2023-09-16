#include "lvgl.h"

#include <memory>
namespace UI::Widget{

class Base{
public:
    typedef std::unique_ptr<Base> Ptr;

    Base(lv_obj_t* aLvglSelf);
    
private:
    lv_obj_t* mLvglSelf;
    
    uint16_t mId;
};

}
