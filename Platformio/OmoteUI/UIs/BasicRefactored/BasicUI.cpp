#include "BasicUI.hpp"
#include "HomeScreen.hpp"
#include "ScreenManager.hpp"

using namespace UI;

BasicUI::BasicUI(std::shared_ptr<HardwareAbstract> aHardware)
    : UIBase(aHardware) {
  Screen::Manager::getInstance().pushScreen(
      std::make_unique<Screen::HomeScreen>());
}