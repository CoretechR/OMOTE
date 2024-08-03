#pragma once
#include "IRInterface.hpp"
#include "Notification.hpp"
#include "PageBase.hpp"
namespace UI::Widget {
class Label;
class Button;
} // namespace UI::Widget

namespace UI::Page {

class IrLearner : public Base {
public:
  IrLearner();
  IrLearner(std::shared_ptr<IRInterface> aIr);
  virtual ~IrLearner();

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  void EnableRx();
  void DisableRx();
  void ClearRxLog();
  void AddRxToLog(std::string aLogEntry);

  std::shared_ptr<IRInterface> mIr;
  Handler<IRInterface::RawIR, std::string> mIRReceiver;
  IRInterface::RawIR mLastCaptured;
  bool mHasData = false;
  int mNumCaptured = 0;
  static constexpr auto WaitingForRxStr = "Waiting To Receive...";
  std::string mRxLogStr;

  Widget::Label *mRxStatusLabel;
  static constexpr auto distBetweenRxButtons = 5;
  Widget::Button *mRxEnable;
  Widget::Button *mRxDisable;
  Widget::Button *mClearLog;
  Widget::Label *mRxLog;
};

} // namespace UI::Page
