#pragma once
#include "Animation.hpp"
#include "WidgetBase.hpp"
#include <memory>
#include <string>

namespace UI::Widget {

class Keyboard : public Base {
public:
  Keyboard(std::function<void(std::string)> aOnUserCompletedTextEntry,
           std::string aPrompt = "");

  void OnAdded(UIElement *aNewParent) override;

  void SetHeight(lv_coord_t aHeight) override;

  void AnimateOut();
  void OnKeyboardAnimatedOut(std::function<void()> aOnKeyboardAnimatedOut) {
    mOnKeyboardAnimatedOut = aOnKeyboardAnimatedOut;
  }

private:
  std::function<void()> mOnKeyboardAnimatedOut;
  std::function<void(std::string)> mOnUserCompleteTextEntry;
  std::unique_ptr<Animation> mAnimateIn;

  Base *mKeyboard;
  Base *mTextArea;
};

} // namespace UI::Widget