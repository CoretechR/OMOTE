#include "List.hpp"
#include "BackgroundScreen.hpp"
using namespace UI;
using namespace UI::Widget;

ListItem::ListItem(lv_obj_t *aListItem, std::function<void()> onItemSelected)
    : UIElement(aListItem, ID()), mSelectedHandler(std::move(onItemSelected)) {}

void ListItem::OnLvglEvent(lv_event_t *anEvent) {
  if (anEvent->code == LV_EVENT_CLICKED) {
    if (mSelectedHandler) {
      mSelectedHandler();
    }
  }
}

List::List()
    : Base(lv_list_create(Screen::BackgroundScreen::getLvInstance()),
           ID::Widgets::List) {
  StopLvglEventHandler();
}

void List::AddItem(std::string aTitle, const char *aSymbol,
                   std::function<void()> onItemSelected) {
  auto lvListItem = lv_list_add_btn(LvglSelf(), aSymbol, aTitle.c_str());
  mListItems.push_back(
      std::make_unique<ListItem>(lvListItem, std::move(onItemSelected)));
  mListItems.back()->SetHeight(lv_pct(20));
}
