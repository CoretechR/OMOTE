#include "KeyPressAbstract.hpp"

KeyPressAbstract::KeyPressAbstract() {}

void KeyPressAbstract::RegisterKeyPressHandler(
    std::function<bool(KeyPressAbstract::KeyEvent)> aKeyEventHandler) {
  mKeyEventHandler = std::move(aKeyEventHandler);
}