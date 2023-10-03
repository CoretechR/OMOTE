#pragma once
#include <mutex>

class LvglMutex {
public:
  [[nodiscard]] static std::lock_guard<std::mutex> lockScope() {
    return std::lock_guard<std::mutex>(mLvglMutex);
  }

  inline static std::mutex mLvglMutex;
};
