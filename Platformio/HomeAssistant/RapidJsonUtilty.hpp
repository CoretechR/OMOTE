#pragma once

#include <string>
#include <vector>

#include "rapidjson/document.h"

std::string ToString(rapidjson::Document &aDoc);

std::string ToPrettyString(rapidjson::Document &aDoc);

const rapidjson::Value *GetNestedField(const rapidjson::Value &aValue,
                                       const std::vector<std::string> &aFields);