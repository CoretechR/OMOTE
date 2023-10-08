#pragma once

#include "PageBase.hpp"
#include "ScreenBase.hpp"
#include <memory>
#include <vector>

namespace UI::Screen {

class Manager {
public:
  static Manager &getInstance();

  void pushScreen(Screen::Base::Ptr aScreen);
  void pushScreen(Screen::Base::Ptr aScreen,
                  lv_scr_load_anim_t aPushAnimationOverride);

  void
  pushPopUp(UI::Page::Base::Ptr aPopUpPage,
            lv_scr_load_anim_t aPushAnimation = LV_SCR_LOAD_ANIM_OVER_LEFT);

  Screen::Base::Ptr popScreen(Screen::Base *aScreenToRemove);

  bool distributeKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent);

private:
  Manager();
  static Manager mManager;

  std::vector<Screen::Base::Ptr> mScreens;
};

} // namespace UI::Screen