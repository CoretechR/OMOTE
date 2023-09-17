#include "ScreenBase.hpp"

using namespace UI::Screen;

Base::Base(ID aId) : UIElement(lv_obj_create(NULL), aId) {}

void Base::AddWidget(Widget::Base::Ptr aWidget) {
  mWidgets.push_back(std::move(aWidget));
}

void Base::Show() {
  lv_scr_load_anim(LvglSelf(), mPushAnimation, 1000, 1000, false);
  UIElement::Show();
}

void Base::SetPushAnimation(lv_scr_load_anim_t aShowAnimation) {
  mPushAnimation = aShowAnimation;
}