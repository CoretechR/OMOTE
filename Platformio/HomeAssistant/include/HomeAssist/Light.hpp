#pragma once
#include "HomeAssist/Entity.hpp"
#include "rapidjson/allocators.h"

namespace HomeAssist {

class Light : public Entity {
public:
  Light(std::shared_ptr<IHomeAssistApi> anApi, std::string aEntityId)
      : Entity(anApi, aEntityId) {
    auto &parseHandlers = mUpdateParser.GetHandlersRef();

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
    static const size_t memPoolSize = 1 * 1024;
    char memPool[memPoolSize];
    MemoryPoolAllocator<CrtAllocator> alloc(memPool, memPoolSize);
    auto BrightnessDoc = std::make_unique<Document>(&alloc);
    BrightnessDoc->SetObject();
    Value brightness(kNumberType);
    brightness.SetInt(aBrightness);
    BrightnessDoc->AddMember("brightness", brightness, alloc);

    CallService("services/light/turn_on", *BrightnessDoc);
  }

private:
  int mLastBrightness = 0;
};

} // namespace HomeAssist