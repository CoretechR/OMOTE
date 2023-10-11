#pragma once

#include "UIElement.hpp"
#include <memory>

namespace UI {

namespace Screen {
class PopUpScreen;
}
} // namespace UI

namespace UI::Widget {

class Base : public UIElement {
  // Classes that Own Widgets
  friend class UI::Screen::PopUpScreen;

public:
  typedef std::unique_ptr<Base> Ptr;

  Base(lv_obj_t *aLvglSelf);
  virtual ~Base() override = default;

protected:
  void OnShow() override{};
  void OnHide() override{};
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override {
    return false;
  };

private:
};

} // namespace UI::Widget
