#include "KeyPressAbstract.hpp"

KeyPressAbstract::KeyPressAbstract(
    std::function<bool(KeyPressAbstract::KeyEvent)> aKeyEventHandler)
    : mKeyEventHandler(std::move(aKeyEventHandler)) {}
