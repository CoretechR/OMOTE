#include "ScreenBase.hpp"

using namespace UI::Screen;

Base::Base(ID aId) : UIElement(lv_obj_create(NULL), aId) {}

void Base::Show() {
  lv_scr_load_anim(LvglSelf(), mPushAnimation, mTransitionAnimationTime,
                   mTransitionDelayTime, false);
  UIElement::OnShow();
}

void Base::SetPushAnimation(lv_scr_load_anim_t aShowAnimation) {
  mPushAnimation = aShowAnimation;
}

uint32_t Base::GetTransitionTime() {
  return mTransitionAnimationTime + mTransitionDelayTime;
}

void Base::SetTransitionTimes(uint32_t aAnimationTime, uint32_t aDelay) {
  mTransitionAnimationTime = aAnimationTime;
  mTransitionDelayTime = aDelay;
}

bool Base::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) { return false; };