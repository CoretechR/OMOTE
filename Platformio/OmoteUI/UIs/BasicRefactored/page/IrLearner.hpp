#pragma once
#include "IRInterface.hpp"
#include "Notification.hpp"
#include "PageBase.hpp"

namespace UI::Widget {
class Label;
}

namespace UI::Page {

class IrLearner : public Base {
public:
  IrLearner();
  IrLearner(std::shared_ptr<IRInterface> aIr);
  virtual ~IrLearner();

  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  std::shared_ptr<IRInterface> mIr;
  Handler<IRInterface::RawIR> mIRReceiver;
  IRInterface::RawIR mLastCaptured;
  bool mHasData = false;

  Widget::Label *mReceivedLabel;
};

} // namespace UI::Page
