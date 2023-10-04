#pragma once
#include <functional>
#include <mutex>
#include <queue>

namespace UI {
class UIBase;
} // namespace UI

class LvglResourceManger {
  friend UI::UIBase;

public:
  static LvglResourceManger &GetInstance() {
    static LvglResourceManger mInstance;
    return mInstance;
  }

  [[nodiscard]] std::scoped_lock<std::mutex> scopeLock() {
    return std::scoped_lock(mLvglMutex);
  }

  void AttemptNow(std::function<void()> aLvglModifierFunction) {
    // Attempt to take mutex and preform op if you can't then queue up.
    if (mLvglMutex.try_lock()) {
      aLvglModifierFunction();
      mLvglMutex.unlock();
    } else {
      QueueForLater(aLvglModifierFunction);
    }
  }
  void QueueForLater(std::function<void()> aLvglModifierFunction) {
    mLvglTasks.push(std::move(aLvglModifierFunction));
  }

protected:
  LvglResourceManger(){};

  void HandleQueuedTasks() {
    auto lock = scopeLock();
    while (!mLvglTasks.empty()) {
      mLvglTasks.front()();
      mLvglTasks.pop();
    }
  }

  std::queue<std::function<void()>> mLvglTasks;
  std::mutex mLvglMutex;
};
