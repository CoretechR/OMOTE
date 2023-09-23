#include "PageBase.hpp"

namespace UI::Page {

class TabView : public Base {
public:
  TabView(ID aId);
  void AddTab(Page::Base::Ptr aPage, std::string aTitle);

protected:
  void OnShow() {}
  void OnHide() {}

private:
  std::vector<Page::Base::Ptr> mTabs;
};

} // namespace UI::Page