#pragma once
#include "UIElement.hpp"
#include "WidgetBase.hpp"

#include <vector>

namespace UI::Page {
class Base : public UIElement {
public:
  typedef std::unique_ptr<Base> Ptr;

  Base(ID aID);
  Base(lv_obj_t *aLvglSelf, ID aID);

  void AddWidget(Widget::Base::Ptr aWidget);

protected:
  void OnShow() override{};
  void OnHide() override{};

private:
  std::vector<Widget::Base::Ptr> mWidgets;
};
} // namespace UI::Page
