#pragma once
#include "DisplayAbstract.h"
#include "PageBase.hpp"

namespace UI::Page {
class DisplaySettings : public Base {
public:
  DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay);

  void OnShow() override{};
  void OnHide() override{};

private:
  std::shared_ptr<DisplayAbstract> mDisplay;
};
} // namespace UI::Page
