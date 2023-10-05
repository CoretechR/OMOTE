#include "keys.hpp"

Keys::Keys() {
  static constexpr auto MaxQueueableKeyPresses = 5;
  mKeyPressQueueHandle = xQueueCreate(MaxQueueableKeyPresses, sizeof(KeyEvent));
  xTaskCreate(KeyGrabberTask, "KeyGrabber", 1024, this, 1, &mKeyGrabbingTask);
  xTaskCreate(KeyProccessor, "KeyProccessor", 4096, this, 1, &mKeyHandlingTask);
}

void Keys::KeyGrabberTask(void *aSelf) {
  auto self = reinterpret_cast<Keys *>(aSelf);
  while (true) {
    self->GrabKeys();
    vTaskDelay(5 / portTICK_PERIOD_MS); // 5 ms between key grabs
  }
}
void Keys::KeyProccessor(void *aSelf) {
  auto self = reinterpret_cast<Keys *>(aSelf);
  while (true) {
    self->HandleKeyPresses();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void Keys::HandleKeyPresses() {
  KeyPressAbstract::KeyEvent eventToHandle;
  while (xQueueReceive(mKeyPressQueueHandle, &eventToHandle, 0) == pdTRUE) {
    if (mKeyEventHandler) {
      mKeyEventHandler(eventToHandle);
    }
  }
};

void Keys::QueueKeyEvent(KeyEvent aJustOccuredKeyEvent) {
  BaseType_t higherPriorityTaskAwoke;
  xQueueSendFromISR(mKeyPressQueueHandle, &aJustOccuredKeyEvent,
                    &higherPriorityTaskAwoke);
};

void Keys::GrabKeys() {
  if (!customKeypad.getKeys()) {
    return; // no activity return early.
  }
  for (int i = 0; i < LIST_MAX; i++) {
    if (customKeypad.key[i].kstate == PRESSED ||
        customKeypad.key[i].kstate == RELEASED) {
      // May need to think about resetting sleep timer in key handler....
      //   standbyTimer =
      //       sleepTimeout; // Reset the sleep timer when a button is
      //       pressed
      auto eventType = customKeypad.key[i].kstate == PRESSED
                           ? KeyEvent::Type::Press
                           : KeyEvent::Type::Release;
      const auto keyChar = customKeypad.key[i].kchar;
      auto stateChange = customKeypad.key[i].stateChanged;
      if (charKeyToKeyIds.count(keyChar) > 0 && stateChange) {
        QueueKeyEvent(KeyEvent(charKeyToKeyIds.at(keyChar), eventType));
      }
    }
  }
}