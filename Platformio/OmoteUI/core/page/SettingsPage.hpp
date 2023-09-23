#include "PageBase.hpp"

namespace UI::Page {
class SettingsPage : public Base {
public:
  SettingsPage();

protected:
  void OnShow() override;
};
} // namespace UI::Page
