#include "ScreenManager.hpp"

using namespace UI::Screen;

Manager Manager::mManager = Manager();

Manager &Manager::getInstance() { return mManager; }

Manager::Manager() {}

void Manager::pushScreen(std::unique_ptr<UI::Screen::Base> aPage) {
  pages.push(std::move(aPage));
}
