#include "HardwareAbstract.hpp"
#include "PageBase.hpp"

namespace UI::Widget {
class Button;
class List;
} // namespace UI::Widget
namespace UI::Page {
class SettingsPage : public Base {
public:
  SettingsPage(std::shared_ptr<HardwareAbstract> aHardware = nullptr);

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override {
    return false;
  };

  void PushDisplaySettings();
  void PushSystemSettings();
  void PushWifiSettings();

protected:
  void OnShow() override{};
  void OnHide() override{};

  Widget::Button *mButton;
  Widget::List *mSettingsList;
  std::shared_ptr<HardwareAbstract> mHardware;
};
} // namespace UI::Page
