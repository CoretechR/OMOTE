#pragma once

#include <string>
#include <vector>

#include "rapidjson/document.h"

class MemConciousAllocator;

using MemConciousDocument =
    rapidjson::GenericDocument<rapidjson::UTF8<>, MemConciousAllocator>;
using MemConciousValue =
    rapidjson::GenericValue<rapidjson::UTF8<>, MemConciousAllocator>;

struct BuffDeleter {
  void operator()(void *buffer);
};

class MemConciousAllocator {
 public:
  using BufferType = std::unique_ptr<void, BuffDeleter>;
  static const bool kNeedFree = false;
  static void Free(void *aVal);
  void *Malloc(size_t aSize);
  void *Realloc(void *originalPtr, size_t originalSize, size_t newSize);

 private:
  std::vector<std::vector<char>> mBuffers;
};

std::string ToString(const rapidjson::Document &aDoc);

std::string ToPrettyString(rapidjson::Document &aDoc);

const MemConciousValue *GetNestedField(const MemConciousValue &aValue,
                                       const std::vector<std::string> &aFields);

MemConciousDocument GetDocument(const std::string &aStringToParse);

std::string ToString(const MemConciousDocument &aDoc);
