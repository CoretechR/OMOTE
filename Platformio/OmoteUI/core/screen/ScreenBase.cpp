#include "ScreenBase.hpp"

using namespace UI::Screen;

Base::Base(){

}

void Base::AddWidget(Widget::Base::Ptr aWidget){
    mWidgets.push_back(std::move(aWidget));
}
