#include "UIElement.hpp"

namespace UI {
UIElement::UIElement(lv_obj_t *aLvglSelf, ID aId)
    : mLvglSelf(aLvglSelf), mId(aId) {
  mLvglSelf->user_data = this;
}

} // namespace UI
