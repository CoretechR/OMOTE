#if false
#pragma once

#include "ControllerData.hpp"
#include "PageBase.hpp"

namespace UI::Widget {
class Button;
class Label;
class Joystick;
}  // namespace UI::Widget

namespace UI::Page {

class CarController : public Base {
 public:
  CarController();
  virtual ~CarController() = default;

 protected:
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

 private:
  // UI Elements
  Widget::Label* mTitle;
  Widget::Joystick* mJoystick;

  ControllerData mControlData;
};

}  // namespace UI::Page
#endif