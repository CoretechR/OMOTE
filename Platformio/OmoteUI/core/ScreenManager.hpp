#include "ScreenBase.hpp"
#include <stack>
#include <memory>

namespace UI::Screen{

class Manager{
public:
    static Manager& getInstance();

    void pushScreen(std::unique_ptr<UI::Screen::Base> aPage);

private:
    Manager();
    static Manager mManager;

    std::stack<std::unique_ptr<UI::Screen::Base>> pages;
};

}