#include "PageBase.hpp"
#include "BackgroundScreen.hpp"

using namespace UI::Page;

Base::Base(ID aID)
    : Base(lv_obj_create(Screen::BackgroundScreen::getLvInstance()), aID) {}

Base::Base(lv_obj_t *aLvglSelf, ID aID) : UIElement(aLvglSelf, aID) {
  SetHeight(lv_pct(100));
  SetWidth(lv_pct(100));
  SetPadding(Padding()); // Set Padding to default
}
