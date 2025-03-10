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
  auto homeScreen = std::make_unique<Screen::HomeScreen>();
  mHomeScreen = homeScreen.get();
  Screen::Manager::getInstance().pushScreen(std::move(homeScreen));

  HardwareFactory::getAbstract().wifi()->begin();
}

void BasicUI::AddPageToHomeScreen(std::unique_ptr<Page::Base> aPageToAdd) {
  mHomeScreen->AddPage(std::move(aPageToAdd));
}

bool BasicUI::GoToPage(ID anId) { return mHomeScreen->GoToPage(anId); }
