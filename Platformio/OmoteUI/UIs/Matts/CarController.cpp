#if false
#include "CarController.hpp"

#include "Button.hpp"
#include "Joystick.hpp"

namespace UI::Page {
// Todo move virtual buttons to the actual buttons on the controller duuu

CarController::CarController()
    : Base(ID::Pages::CarController),
      mJoystick(AddNewElement<UI::Widget::Joystick>(
          [this](auto aX, auto aY) { mControlData.SetJoystickOne(aX, aY); }, 50,
          50, 50, 10)) {}

bool CarController::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  auto isPressed = aKeyEvent.mType == KeyPressAbstract::KeyEvent::Type::Press;
  using Id = KeyPressAbstract::KeyId;
  switch (aKeyEvent.mId) {
    case Id::Aux1:
      mControlData.SetA(isPressed);
      break;
    case Id::Aux2:
      mControlData.SetB(isPressed);
      break;
    case Id::Aux3:
      mControlData.SetX(isPressed);
      break;
    case Id::Aux4:
      mControlData.SetY(isPressed);
      break;
    default:
      break;
  }
  return true;
};

}  // namespace UI::Page

#endif