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
}

IrLearner::~IrLearner() { mIr->disableRx(); }

bool IrLearner::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  using ty = KeyPressAbstract::KeyEvent::Type;
  using id = KeyPressAbstract::KeyId;
  if (aKeyEvent.mType == ty::Press) {
    switch (aKeyEvent.mId) {
    case id::Power:
      SetBgColor(Color::BLACK);
      if (mHasData) {
        mIr->send(mLastCaptured);
      }
      break;
    case id::Aux1:
      SetBgColor(Color::RED);
      mIr->send(IRInterface::constInt64SendTypes::Samsung36, 0x400E00FF);
      break;
    case id::Aux2:
      mIr->enableRx();
      mReceivedLabel->SetText("Enabled RX");
      break;
    case id::Aux3:
      mIr->disableRx();
      mReceivedLabel->SetText("Disabled Rx");
      break;
    case id::Aux4:
      mIr->calibrateTx();
      break;
    }
    return true;
  }
  SetBgColor(Color::GREY);
  return true;
}