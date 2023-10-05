#include "HardwareAbstract.hpp"
#include "PageBase.hpp"

namespace UI::Page {
class SettingsPage : public Base {
public:
  SettingsPage(std::shared_ptr<HardwareAbstract> aHardware = nullptr);

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

  void AddSlider();

protected:
  void OnShow() override;

  std::vector<Widget::Base *> sliders;
  std::shared_ptr<HardwareAbstract> mHardware;
};
} // namespace UI::Page
