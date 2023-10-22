#include "poller.hpp"

#include <functional>
#include <memory>

using namespace std::chrono;
using namespace UI;

poller::poller(std::function<void()> aOnPollCb, milliseconds aPollTime)
    : mIntermittentCallback(std::move(aOnPollCb)) {
  mTimer = lv_timer_create(poller::onPoll, aPollTime.count(), this);
  lv_timer_set_repeat_count(mTimer, -1); // Call forever
}

poller::~poller() {
  if (mTimer) {
    lv_timer_del(mTimer);
    mTimer = nullptr;
  }
}

void poller::onPoll(_lv_timer_t *aTimer) {
  poller *currentPoller = reinterpret_cast<poller *>(aTimer->user_data);

  if (currentPoller->mIntermittentCallback) {
    currentPoller->mIntermittentCallback();
  }
}