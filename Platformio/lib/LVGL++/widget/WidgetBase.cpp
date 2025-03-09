#include "WidgetBase.hpp"
#include "BackgroundScreen.hpp"

using namespace UI;
using namespace UI::Widget;

// Constructor typically used as a container for other widgets
Base::Base(ID anId)
    : UIElement(lv_obj_create(Screen::BackgroundScreen::getLvInstance()),
                anId) {
  SetWidth(lv_pct(100));
  SetHeight(lv_pct(100));
  SetPadding(Padding());
  SetOutline(Outline());
  SetBorder(Border());
  SetBgOpacity(LV_OPA_TRANSP);
}

Base::Base(lv_obj_t *aLvglSelf, ID anId) : UIElement(aLvglSelf, anId) {
  SetWidth(lv_pct(100));
  SetHeight(lv_pct(100));
  SetBorder(Border());
}
