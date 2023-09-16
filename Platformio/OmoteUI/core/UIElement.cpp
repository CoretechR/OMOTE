#include "UIElement.hpp"

namespace UI {
UIElement::UIElement(lv_obj_t *aLvglSelf, ID aId)
    : mLvglSelf(aLvglSelf), mId(aId) {
  mLvglSelf->user_data = this;
}

UIElement::SetBgColor(lv_color_t aColor, lv_style_selector_t aStyle) {
  lv_obj_set_style_bg_color(mLvglSelf, aColor, aStyle);
};

} // namespace UI
