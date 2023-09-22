#pragma once
#include "PageBase.hpp"
#include "ScreenBase.hpp"
#include "TabView.hpp"
namespace UI::Screen {

class HomeScreen : public Base {
public:
  HomeScreen();

  // void SetBgColor(lv_color_t value,
  //                 lv_style_selector_t selector = LV_PART_MAIN) override;

  void AddPage(Page::Base::Ptr aPage);

private:
  Page::TabView mMyTabView;
  lv_obj_t *mTabView;
  std::vector<Page::Base::Ptr> mPages;
};

} // namespace UI::Screen