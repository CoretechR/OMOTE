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

  Light(const rapidjson::Value& aAttributeListVal);
  ~Light();

 private:
  int mBrightness;
  int mColorTemp;
  std::tuple<int, int, int> mRgbColor;
  std::vector<ColorModes> mSupportedModes;
};

Message::Attributes::Light::Light(const rapidjson::Value& aAttributeListVal)
    : mBrightness(INVALIDINT),
      mColorTemp(INVALIDINT),
      mRgbColor({INVALIDINT, INVALIDINT, INVALIDINT}) {
  if (aAttributeListVal.HasMember("brightness") &&
      aAttributeListVal["brightness"].IsInt()) {
    mBrightness = aAttributeListVal["brightness"].GetInt();
  }

  if (aAttributeListVal.HasMember("color_temp") &&
      aAttributeListVal["color_temp"].IsInt()) {
    mColorTemp = aAttributeListVal["color_temp"].GetInt();
  }

  if (aAttributeListVal.HasMember("rgb_color") &&
      aAttributeListVal["rgb_color"].IsArray() &&
      aAttributeListVal["rgb_color"].Size() == 3) {
    const auto& rgbArray = aAttributeListVal["rgb_color"].GetArray();
    mRgbColor = std::make_tuple(rgbArray[0].GetInt(), rgbArray[1].GetInt(),
                                rgbArray[2].GetInt());
  }

  if (aAttributeListVal.HasMember("supported_color_modes") &&
      aAttributeListVal["supported_color_modes"].IsArray()) {
    const auto& modesArray =
        aAttributeListVal["supported_color_modes"].GetArray();
    for (const auto& mode : modesArray) {
      if (mode.IsString()) {
        std::string modeStr = mode.GetString();
        if (modeStr == "brightness") {
          mSupportedModes.push_back(ColorModes::Brightness);
        } else if (modeStr == "color_temp") {
          mSupportedModes.push_back(ColorModes::ColorTemp);
        } else if (modeStr == "rgb") {
          mSupportedModes.push_back(ColorModes::rgb);
        }
      }
    }
  }
}

Message::Attributes::Light::~Light() {}

}  // namespace HomeAssist::WebSocket
