#pragma once

#include <memory>
#include <optional>

#include "HomeAssistant/Api/Rest/Access/HomeAssistApiParser.hpp"
#include "HomeAssistant/Api/Rest/Access/IHomeAssistApi.hpp"
#include "HomeAssistant/RapidJsonUtilty.hpp"
#include "rapidjson/allocators.h"

namespace HomeAssist {

/**
 * @brief Represents an entity in Home Assistant and aims to making
 *        interacting with entities easier.
 *        Including getting and setting states.
 */
class Entity {
 public:
  using AllocatorType = rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>;
  static constexpr auto UpdateDocMemPoolSize = 1024;  // 1KB

  Entity(std::shared_ptr<IHomeAssistApi> anApi, std::string aEntityId)
      : mApi(anApi), mEntityId(aEntityId) {}
  virtual ~Entity() = default;

  bool Refresh() {
    auto response = mApi->SendRequest("states/" + mEntityId);
    rapidjson::StringStream s(response.c_str());
    rapidjson::Reader reader;
    return reader.Parse(s, mRefreshParser);
  }

 protected:
  rapidjson::Document &GetUpdateDoc() {
    if (!mUpdateDoc) {
      CreateUpdateDoc();
    }
    return *mUpdateDoc;
  }

  bool CallService(std::string aApiPath) {
    // No way to allocate the memory needed for the
    // Document storage without an allocator
    if (!mAllocator || !mUpdateDoc) {
      return false;
    }

    rapidjson::Value id(rapidjson::kStringType);
    id.SetString(mEntityId.c_str(), *mAllocator);

    mUpdateDoc->AddMember("entity_id", id, *mAllocator);

    auto updates = mApi->SendUpdate(aApiPath.c_str(), ToString(*mUpdateDoc));
    // TODO consider what to do with updates.
    DeleteUpdateDoc();
    return false;
  }

  HomeAssistApiParser mRefreshParser;
  char *mMemPool = nullptr;
  std::unique_ptr<rapidjson::Document> mUpdateDoc = nullptr;
  std::unique_ptr<AllocatorType> mAllocator = nullptr;

 private:
  void DeleteUpdateDoc() {
    if (mUpdateDoc) {
      mUpdateDoc.reset();
    }
    if (mAllocator) {
      mAllocator.reset();
    }
    delete[] mMemPool;
    mMemPool = nullptr;
  }

  void CreateUpdateDoc(bool aReplaceCurrent = false) {
    using namespace rapidjson;
    if (aReplaceCurrent) {
      DeleteUpdateDoc();
    }

    if (!mAllocator && !mUpdateDoc) {
      mMemPool = new char[UpdateDocMemPoolSize];
      mAllocator =
          std::make_unique<AllocatorType>(mMemPool, UpdateDocMemPoolSize);
      mUpdateDoc = std::make_unique<Document>(mAllocator.get());
      mUpdateDoc->SetObject();
    }
  }

  std::shared_ptr<IHomeAssistApi> mApi;
  std::string mEntityId;
};

}  // namespace HomeAssist