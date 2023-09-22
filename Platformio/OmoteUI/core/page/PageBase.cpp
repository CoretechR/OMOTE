#include "PageBase.hpp"

using namespace UI::Page;

Base::Base(ID aID) : UIElement(lv_obj_create(nullptr), aID) {}

Base::Base(lv_obj_t *aLvglSelf, ID aID) : UIElement(aLvglSelf, aID) {}

void Base::AddWidget(Widget::Base::Ptr aWidget) {
  mWidgets.push_back(std::move(aWidget));
}