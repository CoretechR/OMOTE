#pragma once
#include "PageBase.hpp"

namespace UI::Page {

class Demo : public Base {
public:
  Demo();

  void AddSlider();

  void OnShow() override{};
  void OnHide() override{};
  bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent);

private:
  std::vector<UIElement *> sliders;
};

} // namespace UI::Page