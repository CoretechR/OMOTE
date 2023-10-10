#pragma once
#include "UIElement.hpp"
#include "WidgetBase.hpp"

namespace UI {

class WidgetContainer : public UIElement {
public:
  WidgetContainer(lv_obj_t *aLvglSelf, ID aID);

  template <class ElementTy> ElementTy *AddWidget(Widget::Base::Ptr aWidget);

  Widget::Base *AddWidget(Widget::Base::Ptr aWidget);
  Widget::Base::Ptr RemoveWidget(Widget::Base *aWidgetRefrence);
  size_t GetNumWidgets() { return mWidgets.size(); }

protected:
  /// @brief Forward To widgets that are visible
  void OnShow() override;

  /// @brief Forward To widgets that are visible
  void OnHide() override;

  /// @brief Pass Key Events to widgets until one is handled
  ///        TODO: consider an ability to have a selected widget be the first
  ///        one to get events.
  bool KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  std::vector<Widget::Base::Ptr> mWidgets;
};

template <class ElementTy>
ElementTy *WidgetContainer::AddWidget(Widget::Base::Ptr aWidget) {
  return static_cast<ElementTy *>(AddWidget(std::move(aWidget)));
}

} // namespace UI