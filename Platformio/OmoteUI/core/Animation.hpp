#pragma once
#include <functional>
#include <lvgl.h>

namespace UI {

class Animation {
public:
  Animation(std::function<void(int32_t)> aAnimator, int32_t aAnimationTime,
            int32_t aStart = 0, int32_t aEnd = 100);

  virtual ~Animation();

  void Start();

private:
  lv_anim_t mAnimation;
  std::function<void(int32_t)> mAnimator = nullptr;

  static void AnimatorImpl(lv_anim_t *aSelf, int32_t aNextValue);
};

} // namespace UI