#include "IrLearner.hpp"
#include "Button.hpp"
#include "HardwareFactory.hpp"
#include "Label.hpp"

using namespace UI::Page;

IrLearner::IrLearner() : IrLearner(HardwareFactory::getAbstract().ir()){};

IrLearner::IrLearner(std::shared_ptr<IRInterface> aIr)
    : Base(ID::Pages::IrLearner), mIr(aIr),
      mIRReceiver(mIr->IRRecievedNotification()),
      mRxStatusLabel(AddNewElement<Widget::Label>("Rx Status")),
      mRxEnable(AddNewElement<Widget::Button>([this] { EnableRx(); })),
      mRxDisable(AddNewElement<Widget::Button>([this] { DisableRx(); })),
      mClearLog(AddNewElement<Widget::Button>([this] { ClearRxLog(); })),
      mRxLog(AddNewElement<Widget::Label>(WaitingForRxStr)) {

  mIRReceiver = [this](auto rawIrData, auto humanReadableInfo) {
    mLastCaptured = rawIrData;
    mNumCaptured++;

    auto log =
        "#: " + std::to_string(mNumCaptured) + "\n" + humanReadableInfo + "\n";

    AddRxToLog(log);
    mHasData = true;
  };

  mRxStatusLabel->SetHeight(lv_pct(10));

  for (auto button : {mRxDisable, mRxEnable, mClearLog}) {
    button->SetHeight(lv_pct(10));
    button->SetWidth(lv_pct(80));
  }

  mRxEnable->SetText("ENABLE RX");
  mRxDisable->SetText("DISABLE RX");
  mClearLog->SetText("Clear Log");

  mRxEnable->AlignTo(mRxStatusLabel, LV_ALIGN_OUT_BOTTOM_MID, 0,
                     distBetweenRxButtons);
  mRxDisable->AlignTo(mRxEnable, LV_ALIGN_OUT_BOTTOM_MID, 0,
                      distBetweenRxButtons);
  mClearLog->AlignTo(mRxDisable, LV_ALIGN_OUT_BOTTOM_MID, 0,
                     distBetweenRxButtons);
  mRxLog->AlignTo(mClearLog, LV_ALIGN_OUT_BOTTOM_MID, 0, distBetweenRxButtons);
  mRxLog->SetHeight(LV_SIZE_CONTENT);
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
      EnableRx();
      break;
    case id::Aux3:
      DisableRx();
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

void IrLearner::EnableRx() {
  mIr->enableRx();
  mRxStatusLabel->SetText("Enabled RX");
}

void IrLearner::DisableRx() {
  mIr->disableRx();
  mRxStatusLabel->SetText("Disabled Rx");
}

void IrLearner::ClearRxLog() {
  mRxLogStr.clear();
  mRxLog->SetText(WaitingForRxStr);
}

void IrLearner::AddRxToLog(std::string aLogEntry) {
  mRxLogStr += aLogEntry;
  mRxLog->SetText(mRxLogStr);
}