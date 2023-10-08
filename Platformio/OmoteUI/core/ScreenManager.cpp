#include "ScreenManager.hpp"
#include "PopUpScreen.hpp"

using namespace UI::Screen;

Manager Manager::mManager = Manager();

Manager &Manager::getInstance() { return mManager; }

Manager::Manager() {}

void Manager::pushScreen(Screen::Base::Ptr aScreen,
                         lv_scr_load_anim_t aPushAnimation) {
  aScreen->SetPushAnimation(aPushAnimation);
  pushScreen(std::move(aScreen));
}

void Manager::pushScreen(Screen::Base::Ptr aScreen) {
  mScreens.push(std::move(aScreen));
  mScreens.top()->Show();
}

void Manager::pushPopUp(Page::Base::Ptr aPopUpPage,
                        lv_scr_load_anim_t aPushAnimation) {
  pushScreen(std::make_unique<PopUpScreen>(std::move(aPopUpPage)),
             aPushAnimation);
}

bool Manager::distributeKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  // Send Key Even to top Screen for handling
  return mScreens.top()->KeyEvent(aKeyEvent);
}