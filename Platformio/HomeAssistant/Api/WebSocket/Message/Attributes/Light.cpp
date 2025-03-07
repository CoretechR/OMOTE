#include "HomeAssistant/Api/WebSocket/Message/Attributes/Light.hpp"

using namespace HomeAssist::WebSocket;

Message::Attributes::Light::Light(const MemConciousValue& aAttributeListVal)
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

int Message::Attributes::Light::GetBrightness() { return mBrightness; }

int Message::Attributes::Light::GetColorTemp() { return mColorTemp; }

std::tuple<int, int, int> Message::Attributes::Light::GetRgb() {
  return mRgbColor;
}

std::vector<Message::Attributes::Light::ColorModes>
Message::Attributes::Light::GetSupportedColorModes() {
  return mSupportedModes;
}
