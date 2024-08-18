#pragma once
#include "HomeAssist/Entity.hpp"
namespace HomeAssistant {

class Light : public Entity {
public:
  Light(std::shared_ptr<IHomeAssistApi> anApi, std::string aEntityId)
      : Entity(anApi, aEntityId) {}
  virtual ~Light() = default;

  bool isOn() { return GetState() == std::string("on"); }

  int getBrightness() {
    constexpr auto key = "brightness";
    auto attrs = GetAttribues();
    auto attrsStr = ToString(attrs);
    if (attrs.HasMember(key)) {
      auto &brightness = attrs[key];
      if (brightness.IsInt()) {
        return attrs[key].GetInt();
      }
    }
    return -1;
  }

  void setBrightness(int aBrightness) {
    using namespace rapidjson;
    Document BrightnessDoc;
    BrightnessDoc.SetObject();
    auto &alloc = BrightnessDoc.GetAllocator();
    Value brightness(kNumberType);
    brightness.SetInt(aBrightness);
    BrightnessDoc.AddMember("brightness", brightness, alloc);

    CallService("services/light/turn_on", BrightnessDoc);
  }

private:
};

} // namespace HomeAssistant