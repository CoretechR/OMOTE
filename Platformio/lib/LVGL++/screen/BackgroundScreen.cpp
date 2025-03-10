#include "BackgroundScreen.hpp"

using namespace UI::Screen;

BackgroundScreen *BackgroundScreen::mInstance = nullptr;

BackgroundScreen *BackgroundScreen::GetInstance() {
  if (!mInstance) {
    mInstance = new BackgroundScreen();
  }
  return mInstance;
};

lv_obj_t *BackgroundScreen::getLvInstance() {
  return GetInstance()->LvglSelf();
}

BackgroundScreen::BackgroundScreen() : Base(ID::Screens::Background) {}