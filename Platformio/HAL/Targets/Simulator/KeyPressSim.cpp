#include "KeyPressSim.hpp"

#include <memory>

KeyPressSim::KeyPressSim()
    : mKeyHandlerThread([this] {
        while (true) {
          HandleKeyPresses();
          std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
  std::lock_guard lock(mQueueGaurd);
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
};
