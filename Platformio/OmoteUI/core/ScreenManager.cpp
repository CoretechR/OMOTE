#include "ScreenManager.hpp"
#include "PopUpScreen.hpp"

using namespace UI::Screen;

Manager Manager::mManager = Manager();

Manager &Manager::getInstance() { return mManager; }

Manager::Manager() {}

void Manager::pushScreen(Screen::Base::Ptr aScreen) {
  if (!mScreens.empty()) {
    mScreens.back()->OnHide();
  }
  mScreens.push_back(std::move(aScreen));
  mScreens.back()->Show();
}

void Manager::pushScreen(Screen::Base::Ptr aScreen,
                         lv_scr_load_anim_t aPushAnimation) {
  aScreen->SetPushAnimation(aPushAnimation);
  pushScreen(std::move(aScreen));
}

void Manager::pushPopUp(Page::Base::Ptr aPopUpPage,
                        lv_scr_load_anim_t aPushAnimation) {
  pushScreen(std::make_unique<PopUpScreen>(std::move(aPopUpPage)),
             aPushAnimation);
}

UI::Screen::Base::Ptr Manager::popScreen(Screen::Base *aScreenToRemove) {
  auto screenToPop = std::find_if(
      mScreens.begin(), mScreens.end(), [aScreenToRemove](auto &screenInStack) {
        return screenInStack.get() == aScreenToRemove;
      });

  bool isPopOffTopOfStack = mScreens.end() - 1 == screenToPop;
  Screen::Base::Ptr retVal = nullptr;
  if (screenToPop != mScreens.end()) {
    retVal = std::move(*screenToPop);
    mScreens.erase(screenToPop);
    if (isPopOffTopOfStack) {
      // Make sure to show correct top of stack
      // since we popped the top screen off
      mScreens.back()->Show();
      // Make sure to notify the screen that just got moved off top
      retVal->OnHide();
      // In order to make sure the Transition can complete
      // flag the removed screen to stay alive in case it falls
      // out of scope and gets deleted quickly after this.
      retVal->SetKeepAliveTime(mScreens.back()->GetTransitionTime() + 500);
    }
  }

  return retVal;
}

bool Manager::distributeKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  // Send Key Even to top Screen for handling
  return mScreens.back()->KeyEvent(aKeyEvent);
}