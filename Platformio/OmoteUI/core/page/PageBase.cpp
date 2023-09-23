#include "BackgroundScreen.hpp"
#include "PageBase.hpp"

using namespace UI::Page;

Base::Base(ID aID)
    : Base(lv_obj_create(Screen::BackgroundScreen::getLvInstance()), aID) {}

Base::Base(lv_obj_t *aLvglSelf, ID aID) : UIElement(aLvglSelf, aID) {
  SetHeight(lv_pct(100));
  SetWidth(lv_pct(100));
  lv_obj_set_align(aLvglSelf, LV_ALIGN_CENTER);
}

void Base::AddWidget(Widget::Base::Ptr aWidget) {
  AddElement(aWidget.get());
  mWidgets.push_back(std::move(aWidget));
}