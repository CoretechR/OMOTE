#include "ScreenBase.hpp"

using namespace UI::Screen;

Base::Base(ID aId) : UIElement(lv_obj_create(NULL), aId) {}

void Base::AddWidget(Widget::Base::Ptr aWidget) {
  mWidgets.push_back(std::move(aWidget));
}
