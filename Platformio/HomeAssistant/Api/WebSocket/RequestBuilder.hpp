#pragma once

#include <functional>
#include <memory>
#include <string>

#include "HomeAssistant/Api/WebSocket/Request.hpp"
#include "HomeAssistant/RapidJsonUtilty.hpp"

namespace HomeAssist::WebSocket {

// clang-format off
namespace RequestTypes {
    // Core requests
    static constexpr char AUTH[] = "auth";
    static constexpr char PING[] = "ping";
    static constexpr char GET_CONFIG[] = "get_config";
    static constexpr char GET_SERVICES[] = "get_services";

    // Commands
    static constexpr char EXECUTE_SCRIPT[] = "execute_script";
    static constexpr char FIRE_EVENT[] = "fire_event";
    static constexpr char VALIDATE_CONFIG[] = "validate_config";
    static constexpr char CALL_SERVICE[] = "call_service";
    static constexpr char GET_STATES[] = "get_states";
    static constexpr char SUBSCRIBE_ENTITIES[] = "subscribe_entities";
    static constexpr char TEST_CONDITION[] = "test_condition";

    // Event handling
    static constexpr char SUBSCRIBE_EVENTS[] = "subscribe_events";
    static constexpr char UNSUBSCRIBE_EVENTS[] = "unsubscribe_events";
    static constexpr char SUBSCRIBE_TRIGGER[] = "subscribe_trigger";
    static constexpr char BACKUP_SUBSCRIBE_EVENTS[] = "backup/subscribe_events";

    // Cloud related
    static constexpr char CLOUD_REMOVE_DATA[] = "cloud/remove_data";
    static constexpr char CLOUD_STATUS[] = "cloud/status";

    // Configuration
    static constexpr char CONFIG_ENTRIES_FLOW_PROGRESS[] = "config_entries/flow/progress";
    static constexpr char CONFIG_CORE_DETECT[] = "config/core/detect";
    static constexpr char CONFIG_DEVICE_REGISTRY_LIST[] = "config/device_registry/list";
    static constexpr char CONFIG_DEVICE_REGISTRY_UPDATE[] = "config/device_registry/update";
    static constexpr char CONFIG_ENTITY_REGISTRY_LIST[] = "config/entity_registry/list";
    static constexpr char CONFIG_ENTITY_REGISTRY_GET[] = "config/entity_registry/get";
    static constexpr char CONFIG_ENTITY_REGISTRY_LIST_DISPLAY[] = "config/entity_registry/list_for_display";
    static constexpr char CONFIG_FLOOR_REGISTRY_LIST[] = "config/floor_registry/list";
    static constexpr char CONFIG_AREA_REGISTRY_LIST[] = "config/area_registry/list";

    // Shopping list
    static constexpr char SHOPPING_LIST_ITEMS[] = "shopping_list/items";
    static constexpr char SHOPPING_LIST_ITEMS_ADD[] = "shopping_list/items/add";

    // Frontend
    static constexpr char FRONTEND_SET_USER_DATA[] = "frontend/set_user_data";
    static constexpr char FRONTEND_GET_USER_DATA[] = "frontend/get_user_data";

    // Insteon
    static constexpr char INSTEON_CONFIG_GET[] = "insteon/config/get";
    static constexpr char INSTEON_DEVICE_GET[] = "insteon/device/get";
    static constexpr char INSTEON_DEVICE_ADD[] = "insteon/device/add";

    // Miscellaneous
    static constexpr char MANIFEST_LIST[] = "manifest/list";
    static constexpr char THREAD_ADD_DATASET_TLV[] = "thread/add_dataset_tlv";
    static constexpr char ANALYTICS[] = "analytics";
    static constexpr char ANALYTICS_PREFERENCES[] = "analytics/preferences";
}
// clang-format on

class TriggerBuilder {
 public:
  TriggerBuilder(MemConciousDocument::AllocatorType& aAllocator)
      : mTrigger(rapidjson::kObjectType), mAllocator(aAllocator) {}

  TriggerBuilder& SetPlatform(const std::string& aPlatform) {
    mTrigger.AddMember(
        "platform", MemConciousValue().SetString(aPlatform.c_str(), mAllocator),
        mAllocator);
    return *this;
  }

  TriggerBuilder& SetEntityId(const std::string& aEntityId) {
    mTrigger.AddMember(
        "entity_id",
        MemConciousValue().SetString(aEntityId.c_str(), mAllocator),
        mAllocator);
    return *this;
  }

  TriggerBuilder& SetFromState(const std::string& aFrom) {
    mTrigger.AddMember("from",
                       MemConciousValue().SetString(aFrom.c_str(), mAllocator),
                       mAllocator);
    return *this;
  }

  TriggerBuilder& SetToState(const std::string& aTo) {
    mTrigger.AddMember("to",
                       MemConciousValue().SetString(aTo.c_str(), mAllocator),
                       mAllocator);
    return *this;
  }

  MemConciousValue Build() { return std::move(mTrigger); }

 private:
  MemConciousValue mTrigger;
  MemConciousDocument::AllocatorType& mAllocator;
};

class RequestBuilder {
 public:
  RequestBuilder();
  ~RequestBuilder() = default;

  RequestBuilder& SetType(const std::string& aType);
  RequestBuilder& SetId(int aId);
  RequestBuilder& AddField(const std::string& aKey, const std::string& aValue);
  RequestBuilder& AddField(const std::string& aKey, int aValue);
  RequestBuilder& AddField(const std::string& aKey, bool aValue);
  RequestBuilder& AddTrigger(
      std::function<void(TriggerBuilder&)> aTriggerBuilder);

  std::unique_ptr<Request> BuildUnique();
  Request Build();  // keep original for backward compatibility

  static std::unique_ptr<Request> CreateTriggerSubscription(
      int aId, const std::string& aEntityId, const std::string& aFrom,
      const std::string& aTo);

  private:
  MemConciousDocument mDocument;
  MemConciousDocument::AllocatorType& mAllocator;
};

inline RequestBuilder::RequestBuilder()
    : mDocument(rapidjson::kObjectType), mAllocator(mDocument.GetAllocator()) {}

inline RequestBuilder& RequestBuilder::SetType(const std::string& aType) {
  mDocument.AddMember("type",
                      MemConciousValue().SetString(aType.c_str(), mAllocator),
                      mAllocator);
  return *this;
}

inline RequestBuilder& RequestBuilder::SetId(int aId) {
  mDocument.AddMember("id", aId, mAllocator);
  return *this;
}

inline RequestBuilder& RequestBuilder::AddField(const std::string& aKey,
                                                const std::string& aValue) {
  mDocument.AddMember(MemConciousValue().SetString(aKey.c_str(), mAllocator),
                      MemConciousValue().SetString(aValue.c_str(), mAllocator),
                      mAllocator);
  return *this;
}

inline RequestBuilder& RequestBuilder::AddField(const std::string& aKey,
                                                int aValue) {
  mDocument.AddMember(MemConciousValue().SetString(aKey.c_str(), mAllocator),
                      aValue, mAllocator);
  return *this;
}

inline RequestBuilder& RequestBuilder::AddField(const std::string& aKey,
                                                bool aValue) {
  mDocument.AddMember(MemConciousValue().SetString(aKey.c_str(), mAllocator),
                      aValue, mAllocator);
  return *this;
}

inline RequestBuilder& RequestBuilder::AddTrigger(
    std::function<void(TriggerBuilder&)> aTriggerBuilder) {
  TriggerBuilder builder(mAllocator);
  aTriggerBuilder(builder);
  mDocument.AddMember("trigger", builder.Build(), mAllocator);
  return *this;
}

inline std::unique_ptr<Request> RequestBuilder::BuildUnique() {
  return std::make_unique<Request>(std::move(mDocument));
}

inline Request RequestBuilder::Build() { return Request(std::move(mDocument)); }

inline std::unique_ptr<Request> RequestBuilder::CreateTriggerSubscription(
    int aId, const std::string& aEntityId, const std::string& aFrom,
    const std::string& aTo) {
  return RequestBuilder()
      .SetType(RequestTypes::SUBSCRIBE_TRIGGER)
      .SetId(aId)
      .AddTrigger([&](TriggerBuilder& builder) {
        builder.SetPlatform("state")
            .SetEntityId(aEntityId)
            .SetFromState(aFrom)
            .SetToState(aTo);
      })
      .BuildUnique();
}

}  // namespace HomeAssist::WebSocket
