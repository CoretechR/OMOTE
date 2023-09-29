#pragma once
#include "PageBase.hpp"
#include "ScreenBase.hpp"
#include "TabView.hpp"
#include <string>
namespace UI::Screen {

class HomeScreen : public Base {
public:
  HomeScreen();

  void SetBgColor(lv_color_t value,
                  lv_style_selector_t selector = LV_PART_MAIN) override;

private:
  Page::TabView mTabView;
};

} // namespace UI::Screen