#pragma once
#include "Button.hpp"
#include "PageBase.hpp"
#include "ScreenBase.hpp"

namespace UI::Screen {

/// @brief A Screen that allows easy display of a page that
///        can be dismissed easily by an x
class PopUpScreen : public Base {
public:
  PopUpScreen(UI::Page::Base::Ptr aPage);

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

protected:
  void OnShow() override;
  void OnHide() override;

private:
  UI::Page::Base::Ptr mContentPage;
  std::unique_ptr<Widget::Button> mExitButton;
};

} // namespace UI::Screen