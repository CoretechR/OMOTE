#include "PageBase.hpp"
#include <functional>

namespace UI::Page {
class TabView;

class Tab : public Base {
  friend TabView;

public:
  typedef std::unique_ptr<Tab> Ptr;

  Tab(lv_obj_t *aTab, ID aId);

  void GiveContent(Page::Base::Ptr aContent);
  [[nodiscard]] Base::Ptr TakeContent();

  bool KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

private:
  Base::Ptr mContent;
};

class TabView : public Base {
public:
  TabView(ID aId);
  void AddTab(Page::Base::Ptr aPage, std::string aTitle);

  uint16_t GetCurrentTabIdx();
  void SetCurrentTabIdx(uint16_t aTabToSetActive,
                        lv_anim_enable_t aIsDoAnimation = LV_ANIM_ON);

  bool KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

protected:
  void OnShow() {}
  void OnHide() {}

private:
  void HandleTabChange();
  static void HandleTabChangeImpl(lv_event_t *aTabChangeEvent);

  std::vector<Page::Tab::Ptr> mTabs;
};

} // namespace UI::Page