#include "KeyPressSim.hpp"
#include "SimulatorSPSCQueue.hpp"

#include <memory>

KeyPressSim::KeyPressSim(
    std::function<bool(KeyPressAbstract::KeyEvent)> aKeyHandler)
    : KeyPressAbstract(std::move(aKeyHandler)), mKeyGrabberThread([this] {
        while (true) {
          HandleKeyPresses();
          std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
      }),
      mKeyHandlerThread([this] {
        while (true) {
          GrabKeys();
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
      }){};

void KeyPressSim::GrabKeys() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      auto keyEventType = event.type == SDL_KEYDOWN ? KeyEvent::Type::Press
                                                    : KeyEvent::Type::Release;
      const auto SDLK_key = event.key.keysym.sym;
      if (KeyMap.count(SDLK_key) > 0) {
        QueueKeyEvent(KeyEvent(KeyMap.at(SDLK_key), keyEventType));
      }
    }
  }
};

void KeyPressSim::HandleKeyPresses() {
  std::lock_guard lock(mQueueGaurd);
  while (!mKeyEventQueue.empty()) {
    printf(mKeyEventQueue.front().mType == KeyEvent::Type::Release ? "release"
                                                                   : "press");
    mKeyEventHandler(mKeyEventQueue.front());
    mKeyEventQueue.pop();
  }
};

void KeyPressSim::QueueKeyEvent(KeyEvent aJustOccuredKeyEvent) {
  std::lock_guard lock(mQueueGaurd);
  mKeyEventQueue.push(aJustOccuredKeyEvent);
};
