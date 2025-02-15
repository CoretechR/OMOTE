#pragma once
#include "Notification.hpp"

class SystemStatsInterface {
public:
  using FreeHeapNotificationType = std::shared_ptr<Notification<uint32_t>>;
  virtual ~SystemStatsInterface() = default;
  virtual uint32_t GetFreeHeap() = 0;

  /**
   * @brief Notification that will periodically fire to let handlers know
   *        the amount of heap memory left.
   */
  FreeHeapNotificationType FreeHeapNotification() {
    return mFreeHeapNotification;
  }

protected:
  void SendFreeHeapNotification() {
    mFreeHeapNotification->notify(GetFreeHeap());
  }

private:
  FreeHeapNotificationType mFreeHeapNotification =
      std::make_shared<Notification<uint32_t>>();
};
