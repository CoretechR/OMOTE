#pragma once

#include "HomeAssist/IHomeAssistApi.hpp"
#include "RapidJsonUtilty.hpp"
#include <memory>
#include <optional>
namespace HomeAssistant {

class Entity {
public:
  Entity(std::shared_ptr<IHomeAssistApi> anApi, std::string aEntityId)
      : mApi(anApi), mEntityId(aEntityId) {}
  virtual ~Entity() = default;

  virtual void Update() {
    if (auto info = GetInfo(); info.has_value()) {
      mLatestInfo.CopyFrom(info.value(), mLatestInfo.GetAllocator());
    }
  }

protected:
  std::optional<rapidjson::Document> GetInfo() {
    return mApi->SendRequest("states/" + mEntityId);
  }

  std::string GetState() {
    std::string state = "";
    if (mLatestInfo.HasMember("state") && mLatestInfo["state"].IsString()) {
      state = mLatestInfo["state"].GetString();
    }
    return state;
  }

  rapidjson::Document GetAttribues() {
    rapidjson::Document attrs;
    if (mLatestInfo.HasMember("attributes")) {
      attrs.CopyFrom(mLatestInfo["attributes"], attrs.GetAllocator());
      return attrs;
    }
    return attrs;
  }

  bool CallService(std::string aApiPath, rapidjson::Document &aUpdateDoc) {
    if (!aUpdateDoc.IsObject()) {
      aUpdateDoc.SetObject();
    }
    auto &alloc = aUpdateDoc.GetAllocator();
    rapidjson::Value id(rapidjson::kStringType);
    id.SetString(mEntityId.c_str(), alloc);
    aUpdateDoc.AddMember("entity_id", id, alloc);

    auto updates = mApi->SendUpdate(aApiPath.c_str(), ToString(aUpdateDoc));
    if (updates.has_value()) {
      auto str = ToString(updates.value());
      str += ";";
    }
    // Todo maybe update Latest info based on it
    return updates.has_value();
  }

private:
  std::shared_ptr<IHomeAssistApi> mApi;
  std::string mEntityId;

  rapidjson::Document mLatestInfo;
};

} // namespace HomeAssistant