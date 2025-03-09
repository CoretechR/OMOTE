#include "Label.hpp"
#include "BackgroundScreen.hpp"
#include "Colors.hpp"
#include "LvglResourceManager.hpp"

using namespace UI::Widget;

Label::Label(std::string aText)
    : Base(lv_label_create(UI::Screen::BackgroundScreen::getLvInstance()),
           ID::Widgets::Label) {
  SetText(aText);
}

void Label::SetText(std::string aText) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_label_set_text(LvglSelf(), aText.c_str());
}

void Label::SetLongMode(lv_label_long_mode_t aLongMode) {
  auto lock = LvglResourceManager::GetInstance().scopeLock();
  lv_label_set_long_mode(LvglSelf(), aLongMode);
}