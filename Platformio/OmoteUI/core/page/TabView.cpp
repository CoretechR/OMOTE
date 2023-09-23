#include "BackgroundScreen.hpp"
#include "TabView.hpp"
#include <string>

using namespace UI::Page;

TabView::TabView(ID aId)
    : Base(lv_tabview_create(Screen::BackgroundScreen::getLvInstance(),
                             LV_DIR_TOP, 0),
           aId) {}

void TabView::AddTab(Page::Base::Ptr aPage, std::string aTitle) {
  auto tab = std::make_unique<Base>(
      lv_tabview_add_tab(LvglSelf(), aTitle.c_str()), aPage->GetID());

  tab->AddElement(aPage.get());

  mTabs.push_back(std::move(tab));
}