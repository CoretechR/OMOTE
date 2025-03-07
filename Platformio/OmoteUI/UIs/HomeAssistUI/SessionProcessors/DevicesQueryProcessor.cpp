#include "DevicesQueryProcessor.hpp"

#include "HardwareFactory.hpp"
namespace UI {

bool DevicesQueryProcessor::Null() { return true; }
bool DevicesQueryProcessor::Bool(bool b) { return true; }
bool DevicesQueryProcessor::Int(int i) { return true; }
bool DevicesQueryProcessor::Uint(unsigned u) { return true; }
bool DevicesQueryProcessor::Int64(int64_t i) { return true; }
bool DevicesQueryProcessor::Uint64(uint64_t u) { return true; }
bool DevicesQueryProcessor::Double(double d) { return true; }
bool DevicesQueryProcessor::RawNumber(const Ch* str, rapidjson::SizeType length,
                                      bool copy) {
  return true;
}
bool DevicesQueryProcessor::String(const Ch* str, rapidjson::SizeType length,
                                   bool copy) {
  if (isProcessingEi && entityIdCallback) {
    auto entity = std::string(str, length);
    entityIdCallback(entity);
    // HardwareFactory::getAbstract().debugPrint("%s\n", entity.c_str());
    isProcessingEi = false;
  }
  return true;
}
bool DevicesQueryProcessor::StartObject() { return true; }
bool DevicesQueryProcessor::Key(const Ch* str, rapidjson::SizeType length,
                                bool copy) {
  isProcessingEi = (strncmp(str, "ei", length) == 0);
  return true;
}
bool DevicesQueryProcessor::EndObject(rapidjson::SizeType memberCount) {
  return true;
}
bool DevicesQueryProcessor::StartArray() { return true; }
bool DevicesQueryProcessor::EndArray(rapidjson::SizeType elementCount) {
  return true;
}

void DevicesQueryProcessor::UpdateProgress(size_t aProcessedBytes,
                                           size_t aTotalBytes) {
  if (mPercentCompleteCallback) {
    auto percentComplete =
        (100 * aProcessedBytes + aTotalBytes / 2) / aTotalBytes;
    if (mPercentComplete == percentComplete) {
      return;
    }
    mPercentComplete = percentComplete;
    mPercentCompleteCallback(percentComplete);
  }
}

void DevicesQueryProcessor::Completed(const resultType& aCompletionResult) {
  if (mRequestProcessCompleteCallback) {
    mRequestProcessCompleteCallback(aCompletionResult);
  }
}

void DevicesQueryProcessor::setPercentCompleteCallback(
    std::function<void(uint16_t)> aCallback) {
  mPercentCompleteCallback = std::move(aCallback);
}

void DevicesQueryProcessor::setRequestProcessCompleteCallback(
    std::function<void(const resultType&)> aCallback) {
  mRequestProcessCompleteCallback = std::move(aCallback);
}

}  // namespace UI
