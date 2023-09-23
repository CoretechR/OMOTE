#include "PageBase.hpp"

namespace UI::Page {

class TabView : public Base {
public:
  TabView(ID aId);
  void AddTab(Page::Base::Ptr aPage, std::string aTitle);

  uint16_t GetCurrentTabIdx();
  void SetCurrentTabIdx(uint16_t aTabToSetActive,
                        lv_anim_enable_t aIsDoAnimation = LV_ANIM_ON);

protected:
  void OnShow() {}
  void OnHide() {}

private:
  void HandleTabChange();
  static void HandleTabChangeImpl(lv_event_t *aTabChangeEvent);

  std::vector<Page::Base::Ptr> mTabs;
};

} // namespace UI::Page