#include "TabView.hpp"

#include <string>

#include "BackgroundScreen.hpp"

using namespace UI::Page;

Tab::Tab(lv_obj_t *aTab, Base::Ptr aContent)
    : Base(aTab, aContent->GetID()),
      mContent(AddElement(std::move(aContent))) {}

/////////////////////TabView/////////////////////////////////////

TabView::TabView(ID aId)
    : Base(lv_tabview_create(Screen::BackgroundScreen::getLvInstance()), aId) {}

void TabView::AddTab(Page::Base::Ptr aPage) {
  auto tab = std::make_unique<Tab>(
      lv_tabview_add_tab(LvglSelf(), aPage->GetTitle().c_str()),
      std::move(aPage));

  mTabs.push_back(std::move(tab));
}

uint16_t TabView::GetCurrentTabIdx() {
  return lv_tabview_get_tab_act(LvglSelf());
}

void TabView::SetCurrentTabIdx(uint16_t aTabToSetActive,
                               lv_anim_enable_t aIsDoAnimation) {
  lv_tabview_set_act(LvglSelf(), aTabToSetActive, aIsDoAnimation);
}

void TabView::HandleTabChange() {
  // Call OnShow() for the page we just swapped to in order to
  // Notify the page that it is now showing and the other that the are now
  // hidden
  for (int i = 0; i < mTabs.size(); i++) {
    if (GetCurrentTabIdx() == i) {
      mTabs[i]->OnShow();
    } else {
      mTabs[i]->OnHide();
    }
  }
}

bool TabView::KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) {
  if (OnKeyEvent(aKeyEvent)) {
    return true;
  }
  return mTabs[GetCurrentTabIdx()]->KeyEvent(aKeyEvent);
};

void TabView::OnLvglEvent(lv_event_t *anEvent) {
  if (lv_event_get_code(anEvent) == LV_EVENT_VALUE_CHANGED) {
    HandleTabChange();
  }
}

bool TabView::GoToTab(ID anId) {
  auto tab = std::find_if(mTabs.begin(), mTabs.end(),
                          [anId](auto &tab) { return tab->GetID() == anId; });
  if (tab != mTabs.end()) {
    auto tabIdx = std::distance(mTabs.begin(), tab);
    SetCurrentTabIdx(tabIdx);
    return true;
  }
  return false;
}

void TabView::OnShow() { mTabs[GetCurrentTabIdx()]->OnShow(); }

void TabView::OnHide() { mTabs[GetCurrentTabIdx()]->OnHide(); }