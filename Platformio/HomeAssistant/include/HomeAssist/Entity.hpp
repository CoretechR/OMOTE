#pragma once

#include "HomeAssist/HomeAssistApiParser.hpp"
#include "HomeAssist/IHomeAssistApi.hpp"
#include "RapidJsonUtilty.hpp"
#include <memory>
#include <optional>

namespace HomeAssist {

class Entity {
public:
  Entity(std::shared_ptr<IHomeAssistApi> anApi, std::string aEntityId)
      : mApi(anApi), mEntityId(aEntityId) {}
  virtual ~Entity() = default;

  bool Update() {
    auto response = mApi->SendRequest("states/" + mEntityId);
    rapidjson::StringStream s(response.c_str());
    rapidjson::Reader reader;
    return reader.Parse(s, mUpdateParser);
  }

protected:
  bool CallService(std::string aApiPath, rapidjson::Document &aUpdateDoc) {
    if (!aUpdateDoc.IsObject()) {
      aUpdateDoc.SetObject();
    }
    auto &alloc = aUpdateDoc.GetAllocator();
    rapidjson::Value id(rapidjson::kStringType);
    id.SetString(mEntityId.c_str(), alloc);

    aUpdateDoc.AddMember("entity_id", id, alloc);

    auto updates = mApi->SendUpdate(aApiPath.c_str(), ToString(aUpdateDoc));
    // TODO consider what to do with updates.
    return false;
  }

  HomeAssistApiParser mUpdateParser;

private:
  std::shared_ptr<IHomeAssistApi> mApi;
  std::string mEntityId;

  std::unique_ptr<rapidjson::Document> mLatestInfo;
};

} // namespace HomeAssist