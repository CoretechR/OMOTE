#pragma once
#include "HomeAssist/Entity.hpp"
#include "rapidjson/allocators.h"

namespace HomeAssist {

class Light : public Entity {
public:
  Light(std::shared_ptr<IHomeAssistApi> anApi, std::string aEntityId)
      : Entity(anApi, aEntityId) {
    auto &parseHandlers = mRefreshParser.GetHandlersRef();

    parseHandlers["brightness"] = [this](auto &aValue) {
      if (aValue.IsUint()) {
        mLastBrightness = aValue.GetUint();
        return true;
      }
      return false;
    };
  }
  virtual ~Light() = default;

  bool isOn() { return false; }

  int getBrightness() { return mLastBrightness; }

  void SetBrightness(int aBrightness) {
    using namespace rapidjson;
    auto &brightnessDoc = GetUpdateDoc();
    auto &alloc = brightnessDoc.GetAllocator();

    Value brightness(kNumberType);
    brightness.SetInt(aBrightness);
    brightnessDoc.AddMember("brightness", brightness, alloc);

    CallService("services/light/turn_on");
  }

private:
  int mLastBrightness = 0;
};

} // namespace HomeAssist