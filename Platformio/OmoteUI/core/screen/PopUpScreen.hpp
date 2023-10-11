#pragma once
#include "Button.hpp"
#include "Label.hpp"
#include "PageBase.hpp"
#include "ScreenBase.hpp"

namespace UI::Screen {

/// @brief A Screen that allows easy display of a page that
///        can be dismissed easily by an x
class PopUpScreen : public Base {
public:
  PopUpScreen(UI::Page::Base::Ptr aPage);

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  UI::Page::Base *mContentPage = nullptr;
  Widget::Button *mExitButton = nullptr;
  Widget::Label *mTitle = nullptr;
};

} // namespace UI::Screen