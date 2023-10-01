#include "PageBase.hpp"

namespace UI::Page {
class SettingsPage : public Base {
public:
  SettingsPage();

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

  void AddSlider();

protected:
  void OnShow() override;

  std::vector<Widget::Base *> sliders;
};
} // namespace UI::Page
