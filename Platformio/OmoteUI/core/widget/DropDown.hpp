#pragma once
#include "BackgroundScreen.hpp"
#include "WidgetBase.hpp"
#include <functional>
#include <string>

namespace UI::Widget {

template <typename T> class DropDown : public Base {
public:
  DropDown(std::function<void(T)> aOnItemSelected)
      : Base(lv_dropdown_create(UI::Screen::BackgroundScreen::getLvInstance()),
             ID::Widgets::DropDown),
        mSelectionHandler(aOnItemSelected) {
    lv_dropdown_clear_options(LvglSelf());
  }

  void AddItem(std::string aOptionTitle, T aOptionData) {
    lv_dropdown_add_option(LvglSelf(), aOptionTitle.c_str(),
                           LV_DROPDOWN_POS_LAST);
    mOptionsData.push_back(aOptionData);
  }

  void SetSelected(T aOptionData) {
    for (int i = 0; i < mOptionsData.size(); i++) {
      if (mOptionsData[i] == aOptionData) {
        lv_dropdown_set_selected(LvglSelf(), i);
      }
    }
  }
  // TODO Could Implement a remove Item but need to make sure
  // correct order is retained in data vector.

protected:
  void OnLvglEvent(lv_event_t *anEvent) override {
    if (anEvent->code == LV_EVENT_VALUE_CHANGED) {
      auto idx = lv_dropdown_get_selected(LvglSelf());
      mSelectionHandler(mOptionsData[idx]);
    }
  };

private:
  std::function<void(T)> mSelectionHandler;
  std::vector<T> mOptionsData;
};

} // namespace UI::Widget