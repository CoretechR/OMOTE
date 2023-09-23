#include "BackgroundScreen.hpp"
#include "TabView.hpp"
#include <string>

using namespace UI::Page;

Tab::Tab(lv_obj_t *aTab, ID aId) : Base(aTab, aId) {}

void Tab::GiveContent(Page::Base::Ptr aContent) {
  AddElement(aContent.get());
  mContent = std::move(aContent);
}

Base::Ptr Tab::TakeContent() { return std::move(mContent); }

TabView::TabView(ID aId)
    : Base(lv_tabview_create(Screen::BackgroundScreen::getLvInstance(),
                             LV_DIR_TOP, 0),
           aId) {
  lv_obj_add_event_cb(LvglSelf(), HandleTabChangeImpl, LV_EVENT_VALUE_CHANGED,
                      nullptr);
}

void TabView::AddTab(Page::Base::Ptr aPage, std::string aTitle) {
  auto tab = std::make_unique<Tab>(
      lv_tabview_add_tab(LvglSelf(), aTitle.c_str()), aPage->GetID());

  tab->GiveContent(std::move(aPage));
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
    auto content = mTabs[i]->TakeContent();
    if (GetCurrentTabIdx() == i) {
      content->OnShow();
    } else {
      content->OnHide();
    }
    mTabs[i]->GiveContent(std::move(content));
  }
}

void TabView::HandleTabChangeImpl(lv_event_t *aTabChangeEvent) {
  auto self =
      UIElement::GetElement<TabView *>(lv_event_get_target(aTabChangeEvent));
  if (self) {
    self->HandleTabChange();
  }
}