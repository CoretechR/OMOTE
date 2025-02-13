#include "CarController.hpp"

#include "Button.hpp"
#include "HardwareFactory.hpp"
#include "Joystick.hpp"
#include "Label.hpp"

namespace UI::Page {
// Todo move virtual buttons to the actual buttons on the controller duuu

CarController::CarController()
    : Base(ID::Pages::CarController),
      mTitle(AddNewElement<UI::Widget::Label>("Waiting For Joy...")),
      mJoystick(AddNewElement<UI::Widget::Joystick>(
          [this](auto aX, auto aY) {
            mControlData.SetJoystickOne(aX, aY);
            mTitle->SetText("X:" + std::to_string(aX) +
                            "Y:" + std::to_string(aY));
            HardwareFactory::getAbstract().display()->ForceRefresh();
          },
          50, 10)) {
  mTitle->AlignTo(this, LV_ALIGN_TOP_MID);
  mTitle->SetHeight(lv_pct(10));
  mJoystick->AlignTo(mTitle, LV_ALIGN_OUT_BOTTOM_MID);
}

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
