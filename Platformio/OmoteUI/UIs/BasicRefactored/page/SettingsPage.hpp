#include "HardwareAbstract.hpp"
#include "PageBase.hpp"

namespace UI::Widget{
  class Button;
}
namespace UI::Page {
class SettingsPage : public Base {
public:
  SettingsPage(std::shared_ptr<HardwareAbstract> aHardware = nullptr);

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

  void AddSlider();
  void PushDisplaySettings();

protected:
  void OnShow() override;
  void OnHide() override{};

  std::vector<UIElement *> sliders;
  Widget::Button *mButton;
  std::shared_ptr<HardwareAbstract> mHardware;
};
} // namespace UI::Page