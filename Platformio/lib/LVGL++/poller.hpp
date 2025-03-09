#include "lvgl.h"
#include <chrono>
#include <functional>
#include <memory>

namespace UI {
class poller {
public:
  poller(std::function<void()> aOnPollCb,
         std::chrono::milliseconds pollTime = std::chrono::seconds(5));
  virtual ~poller();

  void setPollPeriod(std::chrono::milliseconds aPollPeriod) {
    lv_timer_set_period(mTimer, aPollPeriod.count());
  }
  inline void pause() { lv_timer_pause(mTimer); }
  inline void resume() { lv_timer_resume(mTimer); }
  inline void reset() { lv_timer_reset(mTimer); }
  inline void runNext() { lv_timer_ready(mTimer); }

private:
  lv_timer_t *mTimer = nullptr;
  std::function<void()> mIntermittentCallback = nullptr;

  // Static function registered to every timers callback to pass this object as
  // context
  static void onPoll(_lv_timer_t *aTimer);
};
} // namespace UI