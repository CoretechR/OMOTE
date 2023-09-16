#pragma once

namespace UI {

class ID {
public:
  static constexpr auto INVALID = 0;

  enum class Screens {
    Home = static_cast<int>(INVALID) + 1,
    INVALID_SCREEN_ID
  };

  enum class Widgets {
    Slider = static_cast<int>(Screens::INVALID_SCREEN_ID) + 1,
    INVALID_WIDGET_ID
  };

  ID() : mId(INVALID){};
  ID(ID::Screens aScreenId) : mId(static_cast<int>(aScreenId)){};
  ID(ID::Widgets aWidgetId) : mId(static_cast<int>(aWidgetId)){};

private:
  const int mId;
};

} // namespace UI