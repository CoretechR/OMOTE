#include "KeyPressSim.hpp"

#include <memory>

KeyPressSim::KeyPressSim()
    : mKeyHandlerThread(
          [this] {  // Delay to avoid issues with thread init issues with SDL
            std::this_thread::sleep_for(std::chrono::seconds(1));
            while (true) {
              HandleKeyPresses();
            }
          }) {
  SDL_AddEventWatch(KeyPressSim::GrabKeyImpl, this);
};

int KeyPressSim::GrabKeyImpl(void *aSelf, SDL_Event *aEvent) {
  reinterpret_cast<KeyPressSim *>(aSelf)->GrabKeys(aEvent);
  return 0;
}

void KeyPressSim::GrabKeys(SDL_Event *aEvent) {
  if (aEvent->type == SDL_KEYDOWN || aEvent->type == SDL_KEYUP) {
    auto keyEventType = aEvent->type == SDL_KEYDOWN ? KeyEvent::Type::Press
                                                    : KeyEvent::Type::Release;
    const auto SDLK_key = aEvent->key.keysym.sym;
    if (KeyMap.count(SDLK_key) > 0) {
      QueueKeyEvent(KeyEvent(KeyMap.at(SDLK_key), keyEventType));
    }
  }
};

void KeyPressSim::HandleKeyPresses() {
  std::unique_lock lock(mQueueGaurd);
  mProcessKeyQueueCondition.wait(lock,
                                 [this] { return !mKeyEventQueue.empty(); });

  while (!mKeyEventQueue.empty()) {
    if (mKeyEventHandler) {
      mKeyEventHandler(mKeyEventQueue.front());
    }
    mKeyEventQueue.pop();
  }
};

void KeyPressSim::QueueKeyEvent(KeyEvent aJustOccuredKeyEvent) {
  std::lock_guard lock(mQueueGaurd);
  mKeyEventQueue.push(aJustOccuredKeyEvent);
  mProcessKeyQueueCondition.notify_one();
};
