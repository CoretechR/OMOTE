#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <numeric>

#include "IProcessMessage.hpp"
#include "SessionProcessors/DevicesQueryProcessor.hpp"

using namespace testing;
using namespace HAL::WebSocket::Json;

class DevicesQueryProcessorTest : public Test {
 protected:
  std::vector<std::string> capturedEntities;
  UI::DevicesQueryProcessor* processor;
  std::unique_ptr<IProcessMessage> messageProcessor;

  void SetUp() override {
    auto process = std::make_unique<UI::DevicesQueryProcessor>(
        [this](const std::string& entityId) {
          capturedEntities.push_back(entityId);
        });
    processor = process.get();
    messageProcessor =
        std::make_unique<IProcessMessage>(nullptr, std::move(process));
  }
};

TEST_F(DevicesQueryProcessorTest, ShouldExtractEntityIds) {
  std::string testJson =
      R"({"id":37,"type":"result","success":true,"result":{"entity_categories":{"0":"config","1":"diagnostic"},"entities":[
        {"ei":"light.lamp","pl":"cync_lights","lb":[],"di":"4041e3f3eed9eea27e2a21b60484a27f","en":"Lamp"},
        {"ei":"light.shower","pl":"tuya","lb":[],"di":"22b11a1d3cfbcb9eb15d1f658de29106","hn":true},
        {"ei":"light.closet","pl":"tuya","lb":[],"di":"8313147ad6262f1162c1580de20d4bed","hn":true}
    ]}})";

  const bool result =
      messageProcessor->ProcessChunk(testJson, testJson.length());
  ASSERT_TRUE(result);

  ASSERT_THAT(capturedEntities,
              ElementsAre("light.lamp", "light.shower", "light.closet"));
}

TEST_F(DevicesQueryProcessorTest, ShouldHandleEmptyEntities) {
  std::string testJson =
      R"({"id":37,"type":"result","success":true,"result":{"entity_categories":{},"entities":[]}})";

  const bool result =
      messageProcessor->ProcessChunk(testJson, testJson.length());
  ASSERT_TRUE(result);

  ASSERT_THAT(capturedEntities, IsEmpty());
}

TEST_F(DevicesQueryProcessorTest, ShouldHandleProgress) {
  bool progressCalled = false;
  uint16_t lastProgress = 0;

  processor->setPercentCompleteCallback([&](uint16_t progress) {
    progressCalled = true;
    lastProgress = progress;
  });

  std::string testJson = R"({"result":{"entities":[{"ei":"light.lamp"}]}})";
  const bool result =
      messageProcessor->ProcessChunk(testJson, testJson.length());
  ASSERT_TRUE(result);

  ASSERT_TRUE(progressCalled);
  ASSERT_GT(lastProgress, 0);
}

TEST_F(DevicesQueryProcessorTest, ShouldHandleChunkedInput) {
  std::string chunk1 =
      R"({"id":37,"type":"result","success":true,"result":{"entity_categories":{"0":"config"},"entities":[)";
  std::string chunk2 = R"({"ei":"light.lamp"},{"ei":"light.shower"})";
  std::string chunk3 = R"(]}})";

  const auto& chunks = {chunk1, chunk2, chunk3};

  auto totalLength = std::accumulate(
      chunks.begin(), chunks.end(), 0,
      [](auto total, auto nextStr) { return total + nextStr.length(); });

  for (const auto& chunk : chunks) {
    const bool result = messageProcessor->ProcessChunk(chunk, totalLength);
    ASSERT_TRUE(result);
  }

  ASSERT_THAT(capturedEntities, ElementsAre("light.lamp", "light.shower"));
}
