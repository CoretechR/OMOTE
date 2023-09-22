#include "TabView.hpp"

using namespace UI::Page;

TabView::TabView(ID aId)
    : Base(lv_tabview_create(nullptr, LV_DIR_TOP, 0), aId) {}

void TabView::AddTab(Page::Base::Ptr aPage) {
  auto tab = std::make_unique<Base>(lv_tabview_add_tab(LvglSelf(), "fake"),
                                    aPage->GetID());
  tab->AddElement(aPage.get());
  mTabs.push_back(std::move(tab));
}