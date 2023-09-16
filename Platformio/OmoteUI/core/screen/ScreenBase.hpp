#include "lvgl.h"
namespace UI::Screen{

class Base {
public:
    Base();
    
private:
    lv_obj_t *mScreen = lv_obj_create(NULL);
};

}