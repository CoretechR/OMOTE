#pragma once
#include <functional>
#include <mutex>
#include <queue>

namespace UI {
class UIBase;
}  // namespace UI
// MatthewColvin/OMOTE#8
class LvglResourceManager {
  friend UI::UIBase;

 public:
  static LvglResourceManager &GetInstance() {
    static LvglResourceManager mInstance;
    return mInstance;
  }

  [[nodiscard]] std::scoped_lock<std::recursive_mutex> scopeLock() {
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
  LvglResourceManager() {};

  void HandleQueuedTasks() {
    auto lock = scopeLock();
    static constexpr auto TasksPerRefresh = 5;
    for (auto i = 0; i < TasksPerRefresh && !mLvglTasks.empty(); i++) {
      mLvglTasks.front()();
      mLvglTasks.pop();
    }
  }

  std::queue<std::function<void()>> mLvglTasks;
  std::recursive_mutex mLvglMutex;
};
