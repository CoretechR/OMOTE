#include "lvgl.h"
#include "WidgetBase.hpp"

#include <vector>
namespace UI::Screen{

class Base {
public:
    typedef std::unique_ptr<Base> Ptr;
    
    Base();
    
    void AddWidget(Widget::Base::Ptr aWidget);

private:
    std::vector<Widget::Base::Ptr> mWidgets;
    lv_obj_t *mScreen = lv_obj_create(NULL);
};

}