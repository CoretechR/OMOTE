#pragma once

#include "UIElement.hpp"
#include <memory>

namespace UI::Page {
class Base;
}

namespace UI::Widget {

class Base : public UIElement {

  friend class UI::Page::Base;

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
