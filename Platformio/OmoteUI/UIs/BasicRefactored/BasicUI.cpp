#include "BasicUI.hpp"
#include "HardwareFactory.hpp"
#include "HomeScreen.hpp"
#include "ScreenManager.hpp"

using namespace UI;

BasicUI::BasicUI() : UIBase() {

  HardwareFactory::getAbstract().keys()->RegisterKeyPressHandler(
      [](auto aKeyEvent) {
        return Screen::Manager::getInstance().distributeKeyEvent(aKeyEvent);
        // Could potentially add a check here and display that a key event was
        // unused.
      });

  Screen::Manager::getInstance().pushScreen(
      std::make_unique<Screen::HomeScreen>());

  HardwareFactory::getAbstract().wifi()->begin();
}