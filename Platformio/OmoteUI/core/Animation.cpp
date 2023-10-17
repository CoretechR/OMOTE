#include "Animation.hpp"

using namespace UI;

Animation::Animation(std::function<void(int32_t)> aAnimator,
                     int32_t aAnimationTime, int32_t aStart, int32_t aEnd)
    : mAnimator(aAnimator), mStart(aStart), mEnd(aEnd) {
  lv_anim_init(&mAnimation);
  mAnimation.user_data = this;
  lv_anim_set_custom_exec_cb(&mAnimation, AnimatorImpl);
  lv_anim_set_time(&mAnimation, aAnimationTime);
}

Animation::~Animation() { lv_anim_custom_del(&mAnimation, AnimatorImpl); }

void Animation::HandleAnimationComplete(
    std::function<void()> onAnimationComplete) {
  mOnComplete = onAnimationComplete;
}

void Animation::Start() {
  lv_anim_set_values(&mAnimation, mStart, mEnd);
  lv_anim_start(&mAnimation);
}

void Animation::Reverse() {
  std::swap(mStart, mEnd);
  Start();
}

//////////////////////// Statics /////////////////////////////////////////

void Animation::AnimatorImpl(lv_anim_t *aSelf, int32_t aNextValue) {
  auto self = reinterpret_cast<Animation *>(aSelf->user_data);
  self->mAnimator(aNextValue);
  // TODO This probably will not support Overshoot animations.
  if (self->mOnComplete && aNextValue == self->mEnd) {
    self->mOnComplete();
  }
}