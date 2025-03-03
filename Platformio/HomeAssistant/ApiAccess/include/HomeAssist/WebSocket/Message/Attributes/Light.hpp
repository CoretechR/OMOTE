#pragma once
#include <tuple>
#include <vector>

#include "HomeAssist/WebSocket/Message/Attributes/Attributes.hpp"
#include "rapidjson/document.h"

namespace HomeAssist::WebSocket {

class Message::Attributes::Light {
 public:
  static constexpr auto INVALIDINT = -1;
  enum class ColorModes { Brightness, ColorTemp, rgb };

  Light(const MemConciousValue& aAttributeListVal);
  ~Light();

  int GetBrightness();
  int GetColorTemp();
  std::tuple<int, int, int> GetRgb();
  std::vector<ColorModes> GetSupportedColorModes();

 private:
  int mBrightness;
  int mColorTemp;
  std::tuple<int, int, int> mRgbColor;
  std::vector<ColorModes> mSupportedModes;
};

}  // namespace HomeAssist::WebSocket
