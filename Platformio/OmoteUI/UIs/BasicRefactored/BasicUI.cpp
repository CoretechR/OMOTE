#include "BasicUI.hpp"
#include "HomeScreen.hpp"
#include "ScreenManager.hpp"

using namespace UI;

BasicUI::BasicUI(std::shared_ptr<HardwareAbstract> aHardware)
    : UIBase(aHardware) {

  aHardware->keys()->RegisterKeyPressHandler([](auto aKeyEvent) {
    return Screen::Manager::getInstance().distributeKeyEvent(aKeyEvent);
    // Could potentially add a check here and display that a key event was
    // unused.
  });

  Screen::Manager::getInstance().pushScreen(
      std::make_unique<Screen::HomeScreen>(aHardware));

  mHardware->wifi()->begin();
}