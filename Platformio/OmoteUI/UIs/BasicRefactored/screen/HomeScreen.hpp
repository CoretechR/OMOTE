#pragma once
#include "HardwareAbstract.hpp"
#include "PageBase.hpp"
#include "ScreenBase.hpp"
#include "TabView.hpp"
#include <string>
namespace UI::Screen {

class HomeScreen : public Base {
public:
  HomeScreen(std::shared_ptr<HardwareAbstract> aHardware = nullptr);

  void SetBgColor(lv_color_t value,
                  lv_style_selector_t selector = LV_PART_MAIN) override;

protected:
  void OnShow() override{};
  void OnHide() override{};
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  Page::TabView *mTabView;
  std::shared_ptr<HardwareAbstract> mHardware = nullptr;
};

} // namespace UI::Screen