#pragma once
#include "UIElement.hpp"
#include "WidgetBase.hpp"

#include <vector>
namespace UI::Page {
class Tab;
class TabView;
class Base : public UIElement {
  friend Tab;     // Allow Tab to Forward all Key Events to its page
  friend TabView; // Allow Tab view to call OnShow and OnHide Since it can show
                  // and Hide pages by swiping
public:
  typedef std::unique_ptr<Base> Ptr;

  Base(ID aID);
  Base(lv_obj_t *aLvglSelf, ID aID);
  virtual ~Base() = default;

  Widget::Base *AddWidget(Widget::Base::Ptr aWidget);
  Widget::Base::Ptr RemoveWidget(Widget::Base *aWidgetRefrence);
  size_t GetNumWidgets() { return mWidgets.size(); }

protected:
  void OnShow() override{};
  void OnHide() override{};

  bool KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  std::vector<Widget::Base::Ptr> mWidgets;
};
} // namespace UI::Page
