#include "DisplaySettings.hpp"

using namespace UI::Page;

DisplaySettings::DisplaySettings(std::shared_ptr<DisplayAbstract> aDisplay)
    : Base(UI::ID::Pages::DisplaySettings), mDisplay(aDisplay) {}