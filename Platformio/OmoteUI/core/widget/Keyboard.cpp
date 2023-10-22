#include "Keyboard.hpp"
#include "BackgroundScreen.hpp"
#include <cmath>

using namespace UI;
using namespace UI::Widget;

Keyboard::Keyboard(std::function<void(std::string)> aOnUserCompletedTextEntry,
                   std::string aPrompt)
    : Base(ID::Widgets::Keyboard),
      mKeyboard(AddElement<Base>(std::make_unique<Base>(
          lv_keyboard_create(LvglSelf()), ID::Widgets::INVALID_WIDGET_ID))),
      mTextArea(AddElement<Base>(std::make_unique<Base>(
          lv_textarea_create(LvglSelf()), ID::Widgets::INVALID_WIDGET_ID))),
      mOnUserCompleteTextEntry(aOnUserCompletedTextEntry) {
  lv_keyboard_set_textarea(mKeyboard->LvglSelf(), mTextArea->LvglSelf());
  if (!aPrompt.empty()) {
    lv_textarea_set_placeholder_text(mTextArea->LvglSelf(), aPrompt.c_str());
  }

  mKeyboard->OnLvglEvent([this](auto aEvent) {
    if (aEvent->code == LV_EVENT_READY) {
      std::string userEnteredText =
          std::string(lv_textarea_get_text(mTextArea->LvglSelf()));
      mOnUserCompleteTextEntry(userEnteredText);
    }
  });
}

void Keyboard::OnAdded(UIElement *aNewParent) {
  auto selfHeight = ceil(aNewParent->GetContentHeight() * 0.60f);
  // Align to final position and get Y for end of animation
  SetHeight(selfHeight);
  AlignTo(aNewParent, LV_ALIGN_BOTTOM_MID);
  auto endAnimationY = GetY();
  auto startAnimationY = aNewParent->GetBottom();

  mAnimateIn = std::make_unique<Animation>([this](auto aY) { SetY(aY); }, 500,
                                           startAnimationY, endAnimationY);
  mAnimateIn->Start();
}

void Keyboard::AnimateOut() {
  if (mOnKeyboardAnimatedOut) {
    mAnimateIn->HandleAnimationComplete(mOnKeyboardAnimatedOut);
  }
  mAnimateIn->Reverse();
};

void Keyboard::SetHeight(lv_coord_t aHeight) {
  Base::SetHeight(aHeight);
  auto txtAreaHight = 33;
  mTextArea->SetHeight(txtAreaHight);
  mKeyboard->SetHeight(GetContentHeight() - txtAreaHight);
  mTextArea->AlignTo(this, LV_ALIGN_TOP_MID);
  mKeyboard->AlignTo(mTextArea, LV_ALIGN_OUT_BOTTOM_MID);
}
