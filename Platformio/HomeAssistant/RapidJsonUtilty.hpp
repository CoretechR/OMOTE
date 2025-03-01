#pragma once

#include <string>

#include "rapidjson/document.h"

std::string ToString(rapidjson::Document &aDoc);

std::string ToPrettyString(rapidjson::Document &aDoc);