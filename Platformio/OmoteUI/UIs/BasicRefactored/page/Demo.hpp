#pragma once
#include "PageBase.hpp"
#include "HardwareAbstract.hpp"

namespace UI::Page{

class Demo : public Base{
public:
    Demo(std::shared_ptr<HardwareAbstract> aHardware);

    void AddSlider();

    void OnShow()override{};
    void OnHide()override{};
    bool OnKeyEvent(KeyPressAbstract::KeyEvent aKeyEvent);

private:
    std::shared_ptr<HardwareAbstract> mHardware;
    std::vector<UIElement *> sliders;
};

}