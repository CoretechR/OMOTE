#include "IrLearner.hpp"
#include "HardwareFactory.hpp"
#include "Label.hpp"

using namespace UI::Page;

IrLearner::IrLearner() : IrLearner(HardwareFactory::getAbstract().ir()){};

IrLearner::IrLearner(std::shared_ptr<IRInterface> aIr)
    : Base(ID::Pages::IrLearner), mIr(aIr),
      mIRReceiver(mIr->IRRecievedNotification()),
      mReceivedLabel(AddElement<Widget::Label>(
          std::make_unique<Widget::Label>("Waiting For Rx"))) {

  mIRReceiver = [this](auto rawIrData) {
    mLastCaptured = rawIrData;
    mNumCaptured++;
    mReceivedLabel->SetText("Got Rx!! #: " + std::to_string(mNumCaptured));
    mHasData = true;
  };
  mIr->enableRx();
}

IrLearner::~IrLearner() { mIr->disableRx(); }

bool IrLearner::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  using ty = KeyPressAbstract::KeyEvent::Type;
  using id = KeyPressAbstract::KeyId;
  if (aKeyEvent.mId == id::Power) {
    if (aKeyEvent.mType == ty::Press) {
      SetBgColor(Color::BLACK);
      if (mHasData) {
        mIr->send(mLastCaptured);
      }
    } else {
      SetBgColor(Color::GREY);
    }
    return true;
  }
  return false;
}