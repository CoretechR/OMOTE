#pragma once

#include "UIElement.hpp"
#include <memory>
namespace UI::Widget {

class Base : public UIElement {
public:
  typedef std::unique_ptr<Base> Ptr;

  Base(lv_obj_t *aLvglSelf);

protected:
  void OnShow() override{};
  void OnHide() override{};

private:
};

} // namespace UI::Widget
