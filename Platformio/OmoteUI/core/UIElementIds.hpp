#pragma once

namespace UI {

class ID {
public:
  static constexpr auto INVALID = 0;

  enum class Screens {
    Background = static_cast<int>(INVALID) + 1,
    Home,
    PopUp,
    INVALID_SCREEN_ID
  };

  enum class Widgets {
    Slider = static_cast<int>(Screens::INVALID_SCREEN_ID) + 1,
    Button,
    Label,
    List,
    BrightnessSlider,
    INVALID_WIDGET_ID
  };

  enum class Pages {
    Settings = static_cast<int>(Widgets::INVALID_WIDGET_ID) + 1,
    DisplaySettings,
    Demo,
    INVALID_PAGE_ID
  };

  ID() : mId(INVALID){};
  ID(ID::Screens aScreenId) : mId(static_cast<int>(aScreenId)){};
  ID(ID::Widgets aWidgetId) : mId(static_cast<int>(aWidgetId)){};
  ID(ID::Pages aPageId) : mId(static_cast<int>(aPageId)){};

private:
  const int mId;
};

} // namespace UI