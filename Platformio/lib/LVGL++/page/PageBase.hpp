#pragma once
#include "UIElement.hpp"

#include <string>
#include <vector>
namespace UI::Screen {
class PopUpScreen;
}
namespace UI::Page {
class Tab;
class TabView;
class Base : public UIElement {
  // Classes that Own Pages
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

  // Override to have a title associated with your page.
  virtual std::string GetTitle() { return ""; };

protected:
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) { return false; };

private:
};

} // namespace UI::Page
