#include "PageBase.hpp"
#include "BackgroundScreen.hpp"

using namespace UI::Page;

Base::Base(ID aID)
    : Base(lv_obj_create(Screen::BackgroundScreen::getLvInstance()), aID) {}

Base::Base(lv_obj_t *aLvglSelf, ID aID) : UIElement(aLvglSelf, aID) {
  SetHeight(lv_pct(100));
  SetWidth(lv_pct(100));
  SetPadding(Padding()); // Set Padding to default
}
// Return non owning refrence to widget
UI::Widget::Base *Base::AddWidget(Widget::Base::Ptr aWidget) {
  AddElement(aWidget.get());
  mWidgets.push_back(std::move(aWidget));
  return mWidgets[mWidgets.size() - 1].get();
}

UI::Widget::Base::Ptr Base::RemoveWidget(Widget::Base *aWidgetRefrence) {
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

bool Base::KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
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

bool Base::OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) { return false; };