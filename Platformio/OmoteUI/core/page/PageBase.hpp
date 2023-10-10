#pragma once
#include "UIElement.hpp"
#include "WidgetBase.hpp"

#include <string>
#include <vector>
namespace UI::Screen {
class PopUpScreen;
}
namespace UI::Page {
class Tab;
class TabView;
class Base : public UIElement {
  friend Tab;     // Allow Tab to Forward all Key Events to its page
  friend TabView; // Allow Tab view to call OnShow and OnHide Since it can show
                  // and Hide pages by swiping
  friend UI::Screen::PopUpScreen; // Allow Pop up Screens pass events to the
                                  // page it owns

public:
  typedef std::unique_ptr<Base> Ptr;

  Base(ID aID);
  Base(lv_obj_t *aLvglSelf, ID aID);
  virtual ~Base() = default;

  template <class ElementTy> ElementTy *AddWidget(Widget::Base::Ptr aWidget);

  Widget::Base *AddWidget(Widget::Base::Ptr aWidget);
  Widget::Base::Ptr RemoveWidget(Widget::Base *aWidgetRefrence);
  size_t GetNumWidgets() { return mWidgets.size(); }

  // Override to have a title associated with your page.
  virtual std::string GetTitle() { return ""; };

protected:
  void OnShow() override{};
  void OnHide() override{};

  bool KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  std::vector<Widget::Base::Ptr> mWidgets;
};

template <class ElementTy>
ElementTy *Base::AddWidget(Widget::Base::Ptr aWidget) {
  return static_cast<ElementTy *>(AddWidget(std::move(aWidget)));
}

} // namespace UI::Page
