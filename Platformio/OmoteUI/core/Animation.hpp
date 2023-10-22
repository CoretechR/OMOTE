#pragma once
#include <functional>
#include <lvgl.h>

namespace UI {

class Animation {
public:
  Animation(std::function<void(int32_t)> aAnimator, int32_t aAnimationTime,
            int32_t aStart = 0, int32_t aEnd = 100);

  virtual ~Animation();

  void HandleAnimationComplete(std::function<void()> onAnimationComplete);

  void Start();
  void Reverse();

private:
  lv_anim_t mAnimation;
  std::function<void(int32_t)> mAnimator = nullptr;
  std::function<void()> mOnComplete = nullptr;
  bool onCompleteCalled = false;

  int32_t mStart = 0;
  int32_t mEnd = 0;

  static void AnimatorImpl(lv_anim_t *aSelf, int32_t aNextValue);
};

} // namespace UI