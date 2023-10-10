#include "WidgetContainer.hpp"

using namespace UI;

WidgetContainer::WidgetContainer(lv_obj_t *aLvglSelf, ID aID)
    : UIElement(aLvglSelf, aID) {}

UI::Widget::Base *WidgetContainer::AddWidget(Widget::Base::Ptr aWidget) {
  AddElement(aWidget.get());
  mWidgets.push_back(std::move(aWidget));
  return mWidgets[mWidgets.size() - 1].get();
}

UI::Widget::Base::Ptr
WidgetContainer::RemoveWidget(Widget::Base *aWidgetRefrence) {
  auto widgetToRemoveIter = std::find_if(
      mWidgets.begin(), mWidgets.end(), [aWidgetRefrence](auto &aWidget) {
        return aWidget.get() == aWidgetRefrence;
      });
  if (widgetToRemoveIter != mWidgets.end()) {
    auto widget = std::move(*widgetToRemoveIter);
    mWidgets.erase(widgetToRemoveIter);
    return widget;
  }
  return nullptr;
}

bool WidgetContainer::KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  if (OnKeyEvent(aKeyEvent)) {
    return true;
  }
  for (auto &widget : mWidgets) {
    auto used = widget->KeyEvent(aKeyEvent);
    if (used) {
      return true;
    }
  }
  return false;
};

void WidgetContainer::OnShow() {
  for (auto &widget : mWidgets) {
    if (widget->IsVisible()) {
      widget->OnShow();
    }
  }
};

void WidgetContainer::OnHide() {
  for (auto &widget : mWidgets) {
    if (widget->IsVisible()) {
      widget->OnHide();
    }
  }
};