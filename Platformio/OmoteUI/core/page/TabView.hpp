#include "PageBase.hpp"
#include <functional>

namespace UI::Page {
class TabView;

class Tab : public Base {
  friend TabView;

public:
  typedef std::unique_ptr<Tab> Ptr;

  Tab(lv_obj_t *aTab, Base::Ptr aContent);

  void OnShow() override{mContent->OnShow();};
  void OnHide() override{mContent->OnHide();};

private:
 Base* mContent;
};

class TabView : public Base {
public:
  TabView(ID aId);
  void AddTab(Page::Base::Ptr aPage);

  uint16_t GetCurrentTabIdx();
  void SetCurrentTabIdx(uint16_t aTabToSetActive,
                        lv_anim_enable_t aIsDoAnimation = LV_ANIM_ON);

  bool KeyEvent(KeyPressAbstract::KeyEvent aKeyEvent) override;

  void OnShow() override;
  void OnHide() override;

protected:
  void OnLvglEvent(lv_event_t *anEvent) override;

private:
  void HandleTabChange();

  std::vector<Page::Tab::Ptr> mTabs;
};

} // namespace UI::Page