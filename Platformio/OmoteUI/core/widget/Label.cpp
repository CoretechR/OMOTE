#include "Label.hpp"
#include "BackgroundScreen.hpp"
#include "Colors.hpp"

using namespace UI::Widget;

Label::Label(std::string aText)
    : Base(lv_label_create(UI::Screen::BackgroundScreen::getLvInstance())) {
  lv_label_set_text(LvglSelf(), aText.c_str());
}