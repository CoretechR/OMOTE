#include "SettingsPage.hpp"

using namespace UI::Page;

SettingsPage::SettingsPage() : Base(ID::Pages::Settings) {
  SetBgColor(lv_color_make(255, 0, 0));
}